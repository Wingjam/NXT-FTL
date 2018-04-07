#include "follower.h"
#include <thread>
#include <future>
#include <iostream>
#include "wrap_around_iterator.h"

using namespace std;
using namespace nxtftl;


follower::follower(buffer_manager<position>* export_buffers,
    int stopDistance,
    unsigned int size_of_internal_buffer,
    unsigned int number_of_points_between_positions)
    :
    communication{},
    brain{ stopDistance },
    size_of_internal_buffer{ size_of_internal_buffer },
    number_of_points_between_positions{ number_of_points_between_positions },
    export_buffers{ export_buffers },
    export_to_buffers_functor{ export_buffers },
    internal_buffer{ vector<position>{ size_of_internal_buffer } },
    internal_iterator{ internal_buffer.begin(), internal_buffer.end() },
    hermite_progress_iterator{ internal_buffer.begin(), internal_buffer.end() },
    buffer_write_fct{ export_buffers, &internal_iterator }
{ }

bool follower::Init()
{
    // Init
    bool connected = communication.connect(communication::BLUETOOF);
    if (!connected)
    {
        cout << "Error while initiating connection to NXT. Closing..." << endl;
        return false;
    }

    leftMotor = communication.initializeMotor(communication::OUT_A);
    rightMotor = communication.initializeMotor(communication::OUT_C);
    
    communication.initializeSensor(leftColorSensor, communication::IN_1);
    communication.initializeSensor(rightColorSensor, communication::IN_2);
    communication.initializeSensor(touchSensor, communication::IN_3);
    communication.initializeSensor(distanceSensor, communication::IN_4);

    // We need to update all sensors at least once before beginning execution, because the first update is always long.
    update_all_sensor();

    move_history.initialize(buffer_write_fct, leftMotor.tacho_count, rightMotor.tacho_count);

    return true;
}

void follower::update_all_sensor()
{
    communication.updateSensorValue(touchSensor);
    communication.updateSensorValue(distanceSensor);
    communication.updateSensorValue(leftColorSensor);
    communication.updateSensorValue(rightColorSensor);
    communication.update_tacho_count(leftMotor);
    communication.update_tacho_count(rightMotor);
}

bool follower::evaluate_distance()
{
    auto beginning = std::chrono::high_resolution_clock::now();

    // Read critical distance sensor
    communication.updateSensorValue(distanceSensor);

    // Check if it is critical that we stop the robot to prevent any damages
    if (brain.check_for_critical_stop(distanceSensor))
    {
        communication.stopMotor(leftMotor);
        communication.stopMotor(rightMotor);
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "distance sensor : " << time.count() << std::endl;

        // The distance was dangerous and the robot was stopped
        return true;
    }

    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
    std::cout << "distance sensor : " << time.count() << std::endl;

    // The distance was safe, nothing done
    return false;
}

void follower::execute()
{
    bool check_distance = true;
    
    while (true)
    {
        auto realbeginning = std::chrono::high_resolution_clock::now();

        if (check_distance)
        {
            // We want to recheck the distance next iteration if the distance was dangerous
            check_distance = evaluate_distance();
        }
        else
        {
            // We didn't check the distance, next time we should do it
            check_distance = true;

            std::future<void> update_sensors = std::async(std::launch::async, [this] {  
                // Read non-critical sensors.
                auto beginning = std::chrono::high_resolution_clock::now();
                this->communication.updateSensorValue(this->touchSensor);
                auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
                std::cout << "touch sensor : " << time.count() << std::endl;
                beginning = std::chrono::high_resolution_clock::now();
                this->communication.updateSensorValue(this->leftColorSensor);
                time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
                std::cout << "lcolor sensor : " << time.count() << std::endl;
                beginning = std::chrono::high_resolution_clock::now();
                this->communication.updateSensorValue(this->rightColorSensor);
                time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
                std::cout << "rcolor sensor : " << time.count() << std::endl;
                beginning = std::chrono::high_resolution_clock::now();
                this->communication.update_tacho_count(this->leftMotor);
                time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
                std::cout << "ltacho sensor : " << time.count() << std::endl;
                beginning = std::chrono::high_resolution_clock::now();
                this->communication.update_tacho_count(this->rightMotor);
                time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
                std::cout << "rtacho sensor : " << time.count() << std::endl;

            });

            auto beginning = std::chrono::high_resolution_clock::now();

            // We can do some computation while the updates of the sensors are not finished
            auto can_continue_computing = [&update_sensors]() {
                bool pred = update_sensors.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
                std::cout << "hermite pred : " << pred << std::endl;
                return update_sensors.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
            };

            // Do some computation for the rest of the available time
            hermite_progress_iterator = hermite.get_points_between_subdivided(
                hermite_progress_iterator,
                internal_iterator,
                export_to_buffers_functor,
                can_continue_computing,
                number_of_points_between_positions
            );

            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
            std::cout << "hermite : " << time.count() << std::endl;

            // Wait for the updates to finish, in case they were not (we couldn't do more computing)
            update_sensors.wait();

            // Process : According to our benchmark, this is constantly 0 ms
            move_history.log_rotation(leftMotor.tacho_count, rightMotor.tacho_count);
            tuple<float, bool>direction = brain.compute_direction(touchSensor, leftColorSensor, rightColorSensor);
            bool needsToStop = get<1>(direction);
            float turn_factor = get<0>(direction);

            // Check if we need to stop the current execution
            if (needsToStop)
            {
                // Exit the loop (disconnect will handle stopping the motors)
                break;
            }

            // We need to take a decision for the robot's next move and send it
            send_decision_to_robot(turn_factor);

            time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - realbeginning);
            std::cout << "total : " << time.count() << std::endl;
        }
    }
}

void follower::send_decision_to_robot(float turn_factor)
{
    // Here we were successful in taking a decision and time has come to send it.

    turn_factor *= TURN_MULTIPLICATOR;
    communication.startMotor(leftMotor, MOTOR_MEDIUM + MOTOR_HIGH * turn_factor); // =
    communication.startMotor(rightMotor, MOTOR_MEDIUM + MOTOR_HIGH * -turn_factor); // =
}

void follower::Run()
{
    cout << "Press touch sensor to begin..." << endl;
    while (!touchSensor.is_pressed)
    {
        communication.updateSensorValue(touchSensor);
    }
    cout << "Starting line follow..." << endl;
    while (touchSensor.is_pressed)
    {
        communication.updateSensorValue(touchSensor);
    }

    // Execute until the robot is asked to stop
    execute();

    communication.disconnect();
}