#pragma once
#include "SensorDto.h"

class TouchSensorDto :
	public SensorDto
{
public:
	bool isPressed;

	TouchSensorDto() = default;
};

