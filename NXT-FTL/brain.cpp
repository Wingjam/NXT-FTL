#include "brain.h"
using namespace nxtftl;


brain::brain(unsigned int stopDistance) : stopDistance(stopDistance)
{
}

std::tuple<int, bool, bool> brain::compute_direction(const touch_sensor_dto touch_sensor, const distance_sensor_dto distance_sensor, color_sensor_dto left_color_sensor, color_sensor_dto right_color_sensor) const
{
	bool needsToStop = false;
	bool needsToBrake = false;
	// The touch sensor stop the robot
	// If touchsensor is touch => stop
	needsToStop |= touch_sensor.is_pressed;

	// The distance before the robot stop
	// If distanceSensor.distance < stopDistance => stop
	needsToBrake |= distance_sensor.distance <= stopDistance;

	// Calculate the direction of the robot
	int direction{ left_color_sensor.value() - right_color_sensor.value() };

	return std::tuple<int, bool, bool>{direction, needsToBrake, needsToStop};
}
