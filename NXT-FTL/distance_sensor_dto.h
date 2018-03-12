#pragma once
#include "sensor_dto.h"

namespace nxtftl
{
	class distance_sensor_dto :
		public sensor_dto
	{
	public:
		unsigned int distance;
		distance_sensor_dto() = default;
	};
}

