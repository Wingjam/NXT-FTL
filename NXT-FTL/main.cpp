#include "brain.h"
#include "communication.h"

using namespace std;
using namespace nxtftl;

int main()
{
    // Init
    communication communication{};
    brain brain { 10 };

	int port = 0;
	while (!communication.connect(communication::BLUETOOF, port) && ++port < 30)
		;

    auto leftMotor = communication.initializeMotor(communication::OUT_A);
    auto rightMotor = communication.initializeMotor(communication::OUT_C);

    auto touchSensor = touch_sensor_dto{};
    auto leftColorSensor = color_sensor_dto{};
    auto rightColorSensor = color_sensor_dto{};
    auto distanceSensor = distance_sensor_dto{};

    //communication.initializeSensor(touchSensor, communication::IN_1);
    communication.initializeSensor(leftColorSensor, communication::IN_1);
    communication.initializeSensor(rightColorSensor, communication::IN_3);
    communication.initializeSensor(distanceSensor, communication::IN_4);


    while (true)
    {
        // Read
        // communication.updateSensorValue(touchSensor);
        communication.updateSensorValue(leftColorSensor);
        communication.updateSensorValue(rightColorSensor);
        communication.updateSensorValue(distanceSensor);

        // Process
        tuple<int, bool> direction = brain.compute_direction(touchSensor, distanceSensor, leftColorSensor, rightColorSensor);

        // Send

        if (!get<1>(direction))
        {
            communication.stopMotor(leftMotor);
            communication.stopMotor(rightMotor);
            break;
        }

		int turn_factor = get<0>(direction);
		if (0 > turn_factor)
		{
			communication.startMotor(leftMotor, 50);
			communication.startMotor(rightMotor, 75);
		}
		else if (0 < turn_factor)
		{
			communication.startMotor(leftMotor, 75);
			communication.startMotor(rightMotor, 50);
		}
		else // turn_factor == 0
		{
			communication.startMotor(leftMotor, 50);
			communication.startMotor(rightMotor, 50);
		}
    }

}