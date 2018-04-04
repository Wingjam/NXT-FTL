#include "follower.h"
#include <future>
#include <iostream>
#include "wrap_around_iterator.h"
#include "hermite.h"

using namespace std;
using namespace nxtftl;


follower::follower(int stopDistance, buffer_manager<position>* export_buffers) : communication{}, brain{ stopDistance }
{
	this->export_buffers = export_buffers;
    internal_buffer = vector<position>{ SIZE_OF_INTERNAL_BUFFER };
}

void follower::Run()
{
	std::chrono::microseconds time_to_take_a_decision = 500ms;
	std::chrono::microseconds safety_time_net = 15ms;

	// Init
	bool connected = communication.connect(communication::BLUETOOF);
	if (!connected)
	{
		cout << "Error while initiating connection to NXT. Closing..." << endl;
		return;
	}

	auto leftMotor = communication.initializeMotor(communication::OUT_A);
	auto rightMotor = communication.initializeMotor(communication::OUT_C);

    wrap_around_iterator internal_iterator { internal_buffer.begin(), internal_buffer.end() };
    wrap_around_iterator hermite_progress_iterator{ internal_buffer.begin(), internal_buffer.end() };
    auto export_buffers_write_fct = [&export_buffers = export_buffers](position pos) { export_buffers->push_back(pos); };

    auto buffer_write_fct = [&export_buffers = export_buffers, &internal_iterator](position pos) {
        export_buffers->push_back(pos);

        *internal_iterator = pos;
        ++internal_iterator;
    };

	long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
	long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
	movement_history movement_history{ buffer_write_fct, left_motor_tacho_count, right_motor_tacho_count };

	hermite hermite{};

	auto touchSensor = touch_sensor_dto{};
	auto leftColorSensor = color_sensor_dto{};
	auto rightColorSensor = color_sensor_dto{};
	auto distanceSensor = distance_sensor_dto{};

	communication.initializeSensor(leftColorSensor, communication::IN_1);
	communication.initializeSensor(rightColorSensor, communication::IN_2);
	communication.initializeSensor(touchSensor, communication::IN_3);
    communication.initializeSensor(distanceSensor, communication::IN_4);

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

	std::chrono::system_clock::time_point due_time_for_decision = std::chrono::system_clock::now() + time_to_take_a_decision;
	while (true)
	{
        auto realbeginning = std::chrono::high_resolution_clock::now();

        auto beginning = std::chrono::high_resolution_clock::now();
        communication.updateSensorValue(touchSensor);
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "touch sensor : " << time.count() << std::endl;

        beginning = std::chrono::high_resolution_clock::now();
		// Read critical distance sensor
		communication.updateSensorValue(distanceSensor);
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "distance sensor : " << time.count() << std::endl;

		// Check if it is critical that we stop the robot to prevent any damages
		if (brain.check_for_critical_stop(distanceSensor))
		{
			communication.stopMotor(leftMotor);
			communication.stopMotor(rightMotor);

			// Do not execute any other actions for as long as the robot is obstructed
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
		movement_history.log_rotation(left_motor_tacho_count, right_motor_tacho_count);

		// Check if we need to stop the current execution
		bool needsToStop = get<1>(direction);
		if (needsToStop)
		{
			// Exit the loop (disconnect will handle stopping the motors)
			break;
		}

		// We need to wait for the time interval before sending the decision to the robot.
		auto can_continue_computing = [due_time_for_decision, safety_time_net] () { 
            bool pred = std::chrono::system_clock::now() <= due_time_for_decision - safety_time_net;
            std::cout << "hermite pred : " << pred << std::endl;
			return pred;
		};

        beginning = std::chrono::high_resolution_clock::now();

        // TODO Do some computation before sleeping
        hermite_progress_iterator = hermite.get_points_between_subdivided(
            hermite_progress_iterator, 
            internal_iterator, 
            export_buffers_write_fct, 
            can_continue_computing, 
            10000
        );

        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginning);
        std::cout << "hermite sensor : " << time.count() << std::endl;

        // Consume the safety time net
		std::this_thread::sleep_until(due_time_for_decision);

		// Here we were successful in taking a decision and time has come to send it.
        // TODO Scale power using direction (The last possibility is log of the returned value)
        // http://www.cplusplus.com/reference/cmath/log10/
        // last hope: have an epsilon to go straight instead of 0 value
		int turn_factor = get<0>(direction);
		if (0 > turn_factor)
		{
			communication.startMotor(leftMotor, -2); // +
			communication.startMotor(rightMotor, 8); // -
		}
		else if (0 < turn_factor)
		{
			communication.startMotor(leftMotor, 8); // +
			communication.startMotor(rightMotor, -2); // -
		}
		else // turn_factor == 0
		{
			communication.startMotor(leftMotor, 5);
			communication.startMotor(rightMotor, 5);
		}

		// We update the due time for the next decision
		// TODO fix
        //due_time_for_decision += time_to_take_a_decision;
        due_time_for_decision = std::chrono::system_clock::now() + time_to_take_a_decision;
	}

	communication.disconnect();
}