#include "follower.h"
#include <thread>
#include <iostream>
#include "wrap_around_iterator.h"

using namespace std;
using namespace nxtftl;


follower::follower(buffer_manager<position>* export_buffers,
    int stopDistance,
    unsigned int size_of_internal_buffer,
    std::chrono::microseconds time_to_take_a_decision,
    std::chrono::microseconds safety_time_net) 
    :
    communication{},
    brain{ stopDistance },
    size_of_internal_buffer{ size_of_internal_buffer },
    time_to_take_a_decision{ time_to_take_a_decision },
    safety_time_net{ safety_time_net },
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

    long initial_left_tacho_count = communication.get_tacho_count(leftMotor);
    long initial_right_tacho_count = communication.get_tacho_count(rightMotor);

    move_history.initialize(buffer_write_fct, initial_left_tacho_count, initial_right_tacho_count);
    
    communication.initializeSensor(leftColorSensor, communication::IN_1);
    communication.initializeSensor(rightColorSensor, communication::IN_2);
    communication.initializeSensor(touchSensor, communication::IN_3);
    communication.initializeSensor(distanceSensor, communication::IN_4);

    // We need to update all sensors at least once before beginning execution, because the first update is always long.
    update_all_sensor();

    return true;
}

void follower::update_all_sensor()
{
    communication.updateSensorValue(touchSensor);
    communication.updateSensorValue(distanceSensor);
    communication.updateSensorValue(leftColorSensor);
    communication.updateSensorValue(rightColorSensor);
    communication.get_tacho_count(leftMotor);
    communication.get_tacho_count(rightMotor);
}

bool follower::is_distance_dangerous()
{
    auto beginning = std::chrono::high_resolution_clock::now();
    // Read critical distance sensor
    communication.updateSensorValue(distanceSensor);
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
    std::cout << "distance sensor : " << time.count() << std::endl;

    // Check if it is critical that we stop the robot to prevent any damages
    if (brain.check_for_critical_stop(distanceSensor))
    {
        communication.stopMotor(leftMotor);
        communication.stopMotor(rightMotor);

        // Do not execute any other actions for as long as the robot is obstructed
        return true;
    }

    return false;
}

void follower::execute()
{
    std::chrono::system_clock::time_point due_time_for_decision = std::chrono::system_clock::now() + time_to_take_a_decision;
    while (true)
    {
        auto realbeginning = std::chrono::high_resolution_clock::now();

        auto beginning = std::chrono::high_resolution_clock::now();
        communication.updateSensorValue(touchSensor);
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "touch sensor : " << time.count() << std::endl;

        if (is_distance_dangerous())
        {
            // Distance was dangerous, we won't continue our computations and we restart the time to take a decision
            due_time_for_decision = std::chrono::system_clock::now() + time_to_take_a_decision;
            continue;
        }

        // Read non-critical sensors.
        // TODO Check the time left before each updates of the sensors

        beginning = std::chrono::high_resolution_clock::now();
        communication.updateSensorValue(leftColorSensor);
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "lcolor sensor : " << time.count() << std::endl;
        beginning = std::chrono::high_resolution_clock::now();
        communication.updateSensorValue(rightColorSensor);
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "rcolor sensor : " << time.count() << std::endl;
        beginning = std::chrono::high_resolution_clock::now();
        long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "ltacho sensor : " << time.count() << std::endl;
        beginning = std::chrono::high_resolution_clock::now();
        long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "rtacho sensor : " << time.count() << std::endl;

        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - realbeginning);
        std::cout << "total : " << time.count() << std::endl;

        // Process : According to our benchmark, this is constantly 0 ms
        tuple<int, bool>direction = brain.compute_direction(touchSensor, leftColorSensor, rightColorSensor);
        move_history.log_rotation(left_motor_tacho_count, right_motor_tacho_count);

        // Check if we need to stop the current execution
        bool needsToStop = get<1>(direction);
        if (needsToStop)
        {
            // Exit the loop (disconnect will handle stopping the motors)
            break;
        }

        // We need to wait for the time interval before sending the decision to the robot.
        auto can_continue_computing = [due_time_for_decision, &safety_time_net = safety_time_net]() {
            bool pred = std::chrono::system_clock::now() <= due_time_for_decision - safety_time_net;
            std::cout << "hermite pred : " << pred << std::endl;
            return pred;
        };

        beginning = std::chrono::high_resolution_clock::now();

        // TODO Do some computation before sleeping
        hermite_progress_iterator = hermite.get_points_between_subdivided(
            hermite_progress_iterator,
            internal_iterator,
            export_to_buffers_functor,
            can_continue_computing,
            10000
        );

        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "hermite sensor : " << time.count() << std::endl;

        // Consume the safety time net
        std::this_thread::sleep_until(due_time_for_decision);

        // Here we were successful in taking a decision and time has come to send it.
        int turn_factor = get<0>(direction);
        send_decision_to_robot(turn_factor);

        // We update the due time for the next decision
        // TODO fix
        //due_time_for_decision += time_to_take_a_decision;
        due_time_for_decision = std::chrono::system_clock::now() + time_to_take_a_decision;
    }
}

void follower::send_decision_to_robot(int turn_factor)
{
    // Here we were successful in taking a decision and time has come to send it.
    // TODO Scale power using direction (The last possibility is log of the returned value)
    // http://www.cplusplus.com/reference/cmath/log10/
    // last hope: have an epsilon to go straight instead of 0 value
    if (turn_factor  < -EPSILON)
    {
        communication.startMotor(leftMotor, MOTOR_LOW); // -
        communication.startMotor(rightMotor, MOTOR_HIGH); // +
    }
    else if (turn_factor > EPSILON)
    {
        communication.startMotor(leftMotor, MOTOR_HIGH); // +
        communication.startMotor(rightMotor, MOTOR_LOW); // -
    }
    else // turn_factor is between [-EPSILON, EPSILON]
    {
        communication.startMotor(leftMotor, MOTOR_MEDIUM); // =
        communication.startMotor(rightMotor, MOTOR_MEDIUM); // =
    }
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