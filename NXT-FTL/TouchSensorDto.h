#pragma once
#include "SensorDto.h"

namespace NXTFTL
{
	class TouchSensorDto :
		public SensorDto
	{
	public:
		bool isPressed;

		TouchSensorDto() = default;
	};
}

