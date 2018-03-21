#include "follower.h"

#include <future>
#include <iostream>
#include <fstream>

using namespace std;
using namespace nxtftl;


follower::follower(int stopDistance) : communication{}, brain{ stopDistance }
{
}

void follower::Run()
{
	const int max_process_time = 300;
	// Init

	communication.connect(communication::BLUETOOF);

	auto leftMotor = communication.initializeMotor(communication::OUT_A);
	auto rightMotor = communication.initializeMotor(communication::OUT_C);

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

	long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
	long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
	movement_history movement_history{ left_motor_tacho_count, right_motor_tacho_count };

	while (true)
	{
		// Read
		communication.updateSensorValue(touchSensor);
		communication.updateSensorValue(leftColorSensor);
		communication.updateSensorValue(rightColorSensor);
		communication.updateSensorValue(distanceSensor);

		tuple<int, bool, bool> direction;
		std::chrono::system_clock::time_point max_wait_time = std::chrono::system_clock::now() + std::chrono::microseconds(max_process_time);
		//std::future<void> updates = std::async(std::launch::async, [&]
		//{
			// Process
			direction = brain.compute_direction(touchSensor, distanceSensor, leftColorSensor, rightColorSensor);

			long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
			long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
			movement_history.log_rotation(left_motor_tacho_count, right_motor_tacho_count);
		//});

		//bool succeeded = std::future_status::ready == updates.wait_until(max_wait_time);
		//auto end = std::chrono::system_clock::now();
		//std::cout << "Remaning time until timeout:" <<
		//	std::chrono::duration_cast<std::chrono::microseconds>(max_wait_time - end).count() <<
		//	"us." << endl;
		//if (!succeeded)
		//{
		//	direction = tuple<int, bool, bool>{ 0, true, false };
		//}

		// Send
		bool needsToStop = get<2>(direction);
		if (needsToStop)
		{
			// Exit the loop (disconnect will handle stopping the motors)
			break;
		}

		bool needsToBrake = get<1>(direction);
		if (needsToBrake)
		{
			communication.stopMotor(leftMotor);
			communication.stopMotor(rightMotor);
			//if (!succeeded)
			//{
			//	// Now that the robot is stop, we can wait for the robot to give us an answer.
			//	updates.wait();
			//}
			continue;
		}

		int turn_factor = get<0>(direction);
		if (0 > turn_factor)
		{
			communication.startMotor(leftMotor, 10, 80);
			communication.startMotor(rightMotor, 20, 120);
		}
		else if (0 < turn_factor)
		{
			communication.startMotor(leftMotor, 20, 120);
			communication.startMotor(rightMotor, 10, 80);
		}
		else // turn_factor == 0
		{
			communication.startMotor(leftMotor, 20, 120);
			communication.startMotor(rightMotor, 20, 120);
		}
	}
	
	ofstream myfile;
	myfile.open("output.txt");
	std::vector<movement_history::position> positions = movement_history.get_positions();
	for (int i = 0; i < positions.size(); ++i)
	{
		myfile << "(" << positions[i].x << "," << positions[i].y << ")";
	}
	myfile.close();

	communication.disconnect();
}