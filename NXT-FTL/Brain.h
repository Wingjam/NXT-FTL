#pragma once
#include "FTLTouchSensor.h"
#include "FTLDistanceSensor.h"
#include "FTLColorSensor.h"
#include <tuple>

class Brain
{
	int stopDistance;
public:
	Brain(int stopDistance);
	~Brain();

	//Compute de direction that the robot need to take to follow the line. Negative represent left and positive reprensent right.
	std::tuple<int, bool> ComputeDirection(FTLTouchSensor touchSensor, FTLDistanceSensor distanceSensor, FTLColorSensor leftColorSensor, FTLColorSensor rightColorSensor);
};

