#pragma once
#include "touch_sensor_dto.h"
#include "distance_sensor_dto.h"
#include "color_sensor_dto.h"
#include <tuple>

namespace nxtftl
{
	class brain
	{
		unsigned int stopDistance;
	public:
		explicit brain(unsigned int stop_distance);
		~brain() = default;

		/// <summary>
		/// Compute the direction that the robot needs to take to follow the line.
		/// </summary>
		/// <param name="touch_sensor">The sensor that stop the motor when pressed.</param>
		/// <param name="left_color_sensor">The left sensor of the robot used to see the line.</param>
		/// <param name="right_color_sensor">The right sensor of the robot used to see the line.</param>
		/// <returns>The direction to go (Negative for left and positive for right), needsToBrake (temporary), needsToStop (permanent)</returns>
		std::tuple<int, bool, bool> compute_direction(const touch_sensor_dto touch_sensor, const distance_sensor_dto distance_sensor, color_sensor_dto left_color_sensor, color_sensor_dto right_color_sensor) const;
	};
}
