#pragma once
#include "sensor_dto.h"

namespace nxtftl
{
	class color_sensor_dto :
		public sensor_dto
	{
	public:
		unsigned int blue_value;
		unsigned int green_value;
		unsigned int red_value;
		color_sensor_dto() = default;

		unsigned int total_value() const { return blue_value + green_value + red_value; }
	};
}

