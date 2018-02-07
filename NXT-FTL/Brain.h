#pragma once
#include "TouchSensorDto.h"
#include "DistanceSensorDto.h"
#include "ColorSensorDto.h"
#include <tuple>

class Brain
{
	int stopDistance;
public:
	Brain(int stopDistance);
	~Brain();

	//Compute de direction that the robot need to take to follow the line. Negative represent left and positive reprensent right.
	std::tuple<int, bool> ComputeDirection(TouchSensorDto touchSensor, DistanceSensorDto distanceSensor, ColorSensorDto leftColorSensor, ColorSensorDto rightColorSensor);
};

