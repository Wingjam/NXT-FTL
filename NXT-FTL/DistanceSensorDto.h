#pragma once
#include "SensorDto.h"

namespace NXTFTL
{
	class DistanceSensorDto :
		public SensorDto
	{
	public:
		unsigned int Distance;
		DistanceSensorDto() = default;
	};
}

