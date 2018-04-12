#include "follower.h"
#include <thread>
#include <future>
#include <iostream>
#include "wrap_around_iterator.h"

using namespace std;
using namespace nxtftl;


follower::follower(buffer_manager<position>* export_buffers,
    int stop_distance,
    unsigned int size_of_internal_buffer,
    unsigned int number_of_points_between_positions)
    :
    communication{},
    brain{ stop_distance },
    size_of_internal_buffer{ size_of_internal_buffer },
    number_of_points_between_positions{ number_of_points_between_positions },
    export_buffers{ export_buffers },
    export_to_buffers_functor{ export_buffers },
    internal_buffer{ vector<position>{ size_of_internal_buffer } },
    internal_iterator_writer{ internal_buffer.begin(), internal_buffer.end() },
    internal_iterator_reader{ internal_buffer.begin(), internal_buffer.end() },
    buffer_write_fct{ export_buffers, &internal_iterator_writer }
{ 
    hermite_progress = std::make_pair(internal_iterator_reader, 0);
}

bool follower::Init()
{
    // Init
    bool connected = communication.connect(communication::BLUETOOF);
    if (!connected)
    {
        cout << "Error while initiating connection to NXT. Closing..." << endl;
        return false;
    }

    left_motor = communication.initialize_motor(communication::OUT_A);
    right_motor = communication.initialize_motor(communication::OUT_C);
    
    communication.initialize_sensor(left_color_sensor, communication::IN_1);
    communication.initialize_sensor(right_color_sensor, communication::IN_2);
    communication.initialize_sensor(touch_sensor, communication::IN_3);
    communication.initialize_sensor(distance_sensor, communication::IN_4);

    // We need to update all sensors at least once before beginning execution, because the first update is always long.
    update_all_sensor();

    move_history.initialize(buffer_write_fct, left_motor.tacho_count, right_motor.tacho_count);

    return true;
}

void follower::update_all_sensor()
{
    communication.update_sensor_value(touch_sensor);
    communication.update_sensor_value(distance_sensor);
    communication.update_sensor_value(left_color_sensor);
    communication.update_sensor_value(right_color_sensor);
    communication.update_tacho_count(left_motor);
    communication.update_tacho_count(right_motor);
}

bool follower::evaluate_distance()
{
    // Read critical distance sensor
    communication.update_sensor_value(distance_sensor);

    // Check if it is critical that we stop the robot to prevent any damages
    if (brain.check_for_critical_stop(distance_sensor))
    {
        communication.stop_motor(left_motor);
        communication.stop_motor(right_motor);

        // The distance was dangerous and the robot was stopped
        return true;
    }

    // The distance was safe, nothing done
    return false;
}

void follower::execute()
{
    bool check_distance = true;
    
    while (true)
    {
        if (check_distance)
        {
            try
            {
                // We want to recheck the distance next iteration if the distance was dangerous
                check_distance = evaluate_distance();
            }
            catch (exception)
            {
                // Something went wrong while reading the sensor, we need to stop
                std::cout << "Something went wrong with the distance sensor." << std::endl;
                break;
            }
        }
        else
        {
            // We didn't check the distance, next time we should do it
            check_distance = true;

            std::future<void> update_sensors = std::async(std::launch::async, [this] {  
                // Read non-critical sensors.
                this->communication.update_sensor_value(this->touch_sensor);
                this->communication.update_sensor_value(this->left_color_sensor);
                this->communication.update_sensor_value(this->right_color_sensor);
                this->communication.update_tacho_count(this->left_motor);
                this->communication.update_tacho_count(this->right_motor);
            });

            // We can do some computation while the updates of the sensors are not finished
            auto can_continue_computing = [&update_sensors]() {
                return update_sensors.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
            };

            // Do some computation for the rest of the available time
            hermite_progress = hermite.get_points_between_subdivided(
                hermite_progress,
                internal_iterator_writer,
                export_to_buffers_functor,
                can_continue_computing,
                number_of_points_between_positions
            );

            // Wait for the updates to finish, in case they were not (we couldn't do more computing)
            update_sensors.wait();

            // Process : According to our benchmark, this is constantly 0 ms
            move_history.log_rotation(left_motor.tacho_count, right_motor.tacho_count);
            tuple<float, bool>direction = brain.compute_direction(touch_sensor, left_color_sensor, right_color_sensor);
            bool needs_to_stop = get<1>(direction);
            float turn_factor = get<0>(direction);

            // Check if we need to stop the current execution
            if (needs_to_stop)
            {
                // Exit the loop (disconnect will handle stopping the motors)
                break;
            }

            // We need to take a decision for the robot's next move and send it
            send_decision_to_robot(turn_factor);
        }
    }
}

void follower::send_decision_to_robot(float turn_factor)
{
    // Send the decision to the robot
    turn_factor *= TURN_MULTIPLICATOR;
    communication.start_motor(left_motor, static_cast<char>(MOTOR_MEDIUM + MOTOR_HIGH * turn_factor)); // =
    communication.start_motor(right_motor, static_cast<char>(MOTOR_MEDIUM + MOTOR_HIGH * -turn_factor)); // =
}

void follower::Run()
{
    cout << "Press touch sensor to begin..." << endl;
    while (!touch_sensor.is_pressed)
    {
        communication.update_sensor_value(touch_sensor);
    }
    cout << "Starting line follow..." << endl;
    while (touch_sensor.is_pressed)
    {
        communication.update_sensor_value(touch_sensor);
    }

    // Execute until the robot is asked to stop
    execute();

    communication.disconnect();
}