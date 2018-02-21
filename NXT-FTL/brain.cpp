#include "brain.h"
using namespace nxtftl;


brain::brain(unsigned int stopDistance) : stopDistance(stopDistance)
{
}

std::tuple<int, bool> brain::compute_direction(const touch_sensor_dto touch_sensor, const distance_sensor_dto distance_sensor, color_sensor_dto left_color_sensor, color_sensor_dto right_color_sensor) const
{
	bool stopping = false;
	// The touch sensor stop the robot
	//if touchsenso is touch => stop
	stopping |= touch_sensor.is_pressed;

	// The distance before the robot stop
	// if distanceSensor.distance < stopDistance => stop
	stopping |= distance_sensor.distance <= stopDistance;

	int direction{ 0 };
	// Calculate the direction of the robot
	direction = left_color_sensor.value() - right_color_sensor.value();

	return std::tuple<int, bool>{direction, !stopping};
}
