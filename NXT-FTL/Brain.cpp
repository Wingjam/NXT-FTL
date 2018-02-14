#include "Brain.h"

Brain::Brain(unsigned int stopDistance) : stopDistance(stopDistance)
{
}

std::tuple<int, bool> Brain::ComputeDirection(TouchSensorDto touchSensor, DistanceSensorDto distanceSensor, ColorSensorDto leftColorSensor, ColorSensorDto rightColorSensor) const
{
	bool stopping = false;
	// The touch sensor stop the robot
	//if touchsenso is touch => stop
	stopping |= touchSensor.isPressed;

	// The distance before the robot stop
	// if distanceSensor.distance < stopDistance => stop
	stopping |= distanceSensor.Distance <= stopDistance;

	int direction{ 0 };
	// Calculate the direction of the robot
	direction = leftColorSensor.TotalValue() - rightColorSensor.TotalValue();

	return std::tuple<int, bool>{direction, !stopping};
}
