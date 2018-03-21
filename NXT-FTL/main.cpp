#include <future>;
#include "brain.h"
#include "communication.h"
#include "movement_history.h"

using namespace std;
using namespace nxtftl;

int main()
{
    // Init
	movement_history movement_history{};
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
	while (touchSensor.is_pressed)
	{
		communication.updateSensorValue(touchSensor);
	}

    while (true)
    {
		std::chrono::system_clock::time_point max_wait_time = std::chrono::system_clock::now() + std::chrono::milliseconds(3000);
		std::future<void> updates = std::async(std::launch::async, [&]
		{
			// Read
			communication.updateSensorValue(touchSensor);
			communication.updateSensorValue(leftColorSensor);
			communication.updateSensorValue(rightColorSensor);
			communication.updateSensorValue(distanceSensor);
		});
		tuple<int, bool, bool> direction;
		bool succeeded = std::future_status::ready == updates.wait_until(max_wait_time);
		std::cout << std::chrono::duration_cast<std::chrono::microseconds>(max_wait_time - std::chrono::system_clock::now()).count();
		if (succeeded)
		{
			// Process
			direction = brain.compute_direction(touchSensor, distanceSensor, leftColorSensor, rightColorSensor);
		}
		else
		{
			direction = tuple<int, bool, bool>{ 0, true, true };
		}

		long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
		long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
		movement_history.log_rotation(left_motor_tacho_count, right_motor_tacho_count);

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
			if (!succeeded)
			{
				// Now that the robot is stop, we can wait for the robot to give us an answer.
				updates.wait();
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
