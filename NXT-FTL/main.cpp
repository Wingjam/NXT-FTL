#include "Brain.h"
#include "communication.h"

using namespace std;

int main()
{
    // Init
    communication communication{};
    Brain brain { 10 };

    communication.connect(communication::BLUETOOF, 40);

    auto leftMotor = communication.initializeMotor(communication::OUT_A);
    auto rightMotor = communication.initializeMotor(communication::OUT_C);

    auto touchSensor = TouchSensorDto{};
    auto leftColorSensor = ColorSensorDto{};
    auto rightColorSensor = ColorSensorDto{};
    auto distanceSensor = DistanceSensorDto{};

    communication.initializeSensor(touchSensor, communication::IN_1);
    communication.initializeSensor(leftColorSensor, communication::IN_2);
    communication.initializeSensor(rightColorSensor, communication::IN_3);
    communication.initializeSensor(distanceSensor, communication::IN_4);


    while (true)
    {
        // Read
        communication.updateSensorValue(touchSensor);
        communication.updateSensorValue(leftColorSensor);
        communication.updateSensorValue(rightColorSensor);
        communication.updateSensorValue(distanceSensor);

        // Process
        tuple<int, bool> direction = brain.ComputeDirection(touchSensor, distanceSensor, leftColorSensor, rightColorSensor);

        // Send

        if (!get<1>(direction))
        {
            communication.stopMotor(leftMotor);
            communication.stopMotor(rightMotor);
            break;
        }

        communication.startMotor(leftMotor, 75);
        communication.startMotor(rightMotor, 75);
    }

}