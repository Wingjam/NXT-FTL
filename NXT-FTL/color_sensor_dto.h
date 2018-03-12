#pragma once
#include "sensor_dto.h"

namespace nxtftl
{
	class color_sensor_dto :
		public sensor_dto
	{
	public:
		int intensity;
		color_sensor_dto() = default;

		int value() const { return intensity; }
	};
}

