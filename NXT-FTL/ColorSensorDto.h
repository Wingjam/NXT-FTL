#pragma once
#include "SensorDto.h"

namespace NXTFTL
{
	class ColorSensorDto :
		public SensorDto
	{
	public:
		unsigned int BlueValue;
		unsigned int GreenValue;
		unsigned int RedValue;
		ColorSensorDto() = default;

		unsigned int TotalValue() const { return BlueValue + GreenValue + RedValue; }
	};
}

