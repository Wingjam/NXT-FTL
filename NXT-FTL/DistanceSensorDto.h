#pragma once
#include "SensorDto.h"

class DistanceSensorDto :
	public SensorDto
{
public:
	unsigned int Distance;
	DistanceSensorDto() = default;
};

