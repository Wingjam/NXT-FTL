#pragma once
#include "sensor_dto.h"

namespace nxtftl
{
	class touch_sensor_dto :
		public sensor_dto
	{
	public:
		bool is_pressed;

		touch_sensor_dto() = default;
	};
}

