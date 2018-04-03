#pragma once

#include <chrono>
#include <vector>
#include <cmath>
#include "position.h"
#include <iostream>
#include <fstream>
#include <functional>

class movement_history
{
	const float WHEEL_DIAMETER = 4.4f;
	const float PI = 3.1416f;
	const float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * PI;
	const float DISTANCE_BETWEEN_WHEELS = 16.9f;
	const float WHEEL_WIDTH = 2.2f;
	
public:
private:
	struct snapshot {
		long int left_motor_tacho_count;
		long int right_motor_tacho_count;
	};

    std::function<void(position)> buffer_write_fct;
	snapshot last_snapshot;
	position last_position;
	position movement_history::calculate_new_position(position initial_position, snapshot initial_snapshot, snapshot destination_snapshot);

public:
	movement_history(std::function<void(position)> buffer_write_fct, long int initial_left_motor_tacho_count, long int initial_right_motor_tacho_count);
	~movement_history() = default;
	void log_rotation(long int left_motor_tacho_count, long int right_motor_tacho_count);
	position get_current_position();
};

