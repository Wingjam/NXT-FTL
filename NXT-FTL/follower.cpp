#include "follower.h"

#include <future>
#include <iostream>

using namespace std;
using namespace nxtftl;


follower::follower(int stopDistance, buffer_manager<position>* buffers) : communication{}, brain{ stopDistance }
{
	this->buffers = buffers;
}

void follower::Run()
{
	std::chrono::microseconds time_to_take_a_decision = 100ms;
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

	long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
	long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
	movement_history movement_history{ left_motor_tacho_count, right_motor_tacho_count };

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
		// Read critical distance sensor
		communication.updateSensorValue(distanceSensor);

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
		communication.updateSensorValue(touchSensor);
		communication.updateSensorValue(leftColorSensor);
		communication.updateSensorValue(rightColorSensor);
		long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
		long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);

		tuple<int, bool> direction;
		std::future<void> updates = std::async(std::launch::async, [&]
		{
			// Process
			direction = brain.compute_direction(touchSensor, leftColorSensor, rightColorSensor);
			movement_history.log_rotation(left_motor_tacho_count, right_motor_tacho_count);
		});

		bool succeeded = std::future_status::ready == updates.wait_until(due_time_for_decision - safety_time_net);
		//auto end = std::chrono::system_clock::now();
		//std::cout << "Remaning time until timeout:" <<
		//	std::chrono::duration_cast<std::chrono::microseconds>(max_wait_time - end).count() <<
		//	"us." << endl;
		if (!succeeded) // It took too much time to take a decision and calculates our points
		{
			communication.stopMotor(leftMotor);
			communication.stopMotor(rightMotor);

			// We need to finish the computations before progressing. We can't stop them forcibly because they are not stateless.
			updates.wait();

			// We consider the result of the updates to no longer be relevant, so we will begin the acquisition and decision processes again.
			due_time_for_decision = std::chrono::system_clock::now() + time_to_take_a_decision;
			continue;
		}

		// Check if we need to stop the current execution
		bool needsToStop = get<1>(direction);
		if (needsToStop)
		{
			// Exit the loop (disconnect will handle stopping the motors)
			break;
		}

		// We need to wait for the time interval before sending the decision to the robot.
		// TODO Do some computation before sleeping
		auto can_continue_computing = [due_time_for_decision, safety_time_net] () { 
			return std::chrono::system_clock::now() <= due_time_for_decision - safety_time_net;
		};

		//hermite.get_points_between_subdivided()


		std::this_thread::sleep_until(due_time_for_decision);

		// Here we were successful in taking a decision and time has come to send it.
        // TODO Scale power using direction
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
		due_time_for_decision += time_to_take_a_decision;
	}

	communication.disconnect();

	std::ofstream myfile;
	myfile.open("output.txt");
	movement_history.write_positions_to_stream(myfile);
	myfile.close();
}