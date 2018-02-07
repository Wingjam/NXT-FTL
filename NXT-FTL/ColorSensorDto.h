#pragma once
#include "SensorDto.h"
class ColorSensorDto :
	public SensorDto
{
public:
	unsigned int BlueValue;
	unsigned int GreenValue;
	unsigned int RedValue;
	ColorSensorDto() = default;
};

