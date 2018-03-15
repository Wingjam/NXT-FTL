#include <future>;
#include "brain.h"
#include "communication.h"

using namespace std;
using namespace nxtftl;

int main()
{
    // Init
    communication communication{};
    brain brain { 10 };

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

    while (true)
    {
		std::chrono::system_clock::time_point max_wait_time = std::chrono::system_clock::now() + std::chrono::milliseconds(30);
		std::future<void> update_touch = std::async(std::launch::async, [&]
		{
			// Read
			communication.updateSensorValue(touchSensor);
		});
		std::future<void> update_left = std::async(std::launch::async, [&]
		{
			// Read
			communication.updateSensorValue(leftColorSensor);
		});
		std::future<void> update_right = std::async(std::launch::async, [&]
		{
			// Read
			communication.updateSensorValue(rightColorSensor);
		});
		std::future<void> update_distance = std::async(std::launch::async, [&]
		{
			// Read
			communication.updateSensorValue(distanceSensor);
		});
		tuple<int, bool> direction;
		bool succeeded = std::future_status::ready == update_touch.wait_until(max_wait_time);
		succeeded &= std::future_status::ready == update_left.wait_until(max_wait_time);
		succeeded &= std::future_status::ready == update_right.wait_until(max_wait_time);
		succeeded &= std::future_status::ready == update_distance.wait_until(max_wait_time);
		if (succeeded)
		{
			// Process
			direction = brain.compute_direction(touchSensor, distanceSensor, leftColorSensor, rightColorSensor);
		}
		else
		{
			direction = tuple<int, bool>{ 0, false };
		}

        // Send
        bool can_move = get<1>(direction);
        if (!can_move)
        {
            communication.stopMotor(leftMotor);
            communication.stopMotor(rightMotor);
			if (!succeeded)
			{
				// Now that the robot is stop, we can wait for the robot to give us an answer.
				update_touch.wait();
				update_left.wait();
				update_right.wait();
				update_distance.wait();
			}
            continue;
        }
        
        int turn_factor = get<0>(direction);
        if (0 > turn_factor)
		{
			communication.startMotor(leftMotor, 10);
			communication.startMotor(rightMotor, 20);
		}
		else if (0 < turn_factor)
		{
			communication.startMotor(leftMotor, 20);
			communication.startMotor(rightMotor, 10);
		}
		else // turn_factor == 0
		{
			communication.startMotor(leftMotor, 10);
			communication.startMotor(rightMotor, 10);
		}
    }

	communication.disconnect();
}