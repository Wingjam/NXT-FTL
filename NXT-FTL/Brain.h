#pragma once
#include "TouchSensorDto.h"
#include "DistanceSensorDto.h"
#include "ColorSensorDto.h"
#include <tuple>

namespace NXTFTL
{
	class Brain
	{
		unsigned int stopDistance;
	public:
		Brain(unsigned int stopDistance);
		~Brain() = default;

		//Compute de direction that the robot need to take to follow the line. Negative represent left and positive reprensent right. True to move, false to stop
		std::tuple<int, bool> ComputeDirection(TouchSensorDto touchSensor, DistanceSensorDto distanceSensor, ColorSensorDto leftColorSensor, ColorSensorDto rightColorSensor) const;
	};
}
