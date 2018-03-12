#pragma once

#include <chrono>
#include <vector>

class movement_history
{
	struct snapshot {
		std::chrono::milliseconds ms;
		long int left_motor_tacho_count;
		long int right_motor_tacho_count;
	};

	std::vector<snapshot> snapshots;
	
public:
	movement_history();
	~movement_history();
	void log_rotation(long int left_motor_tacho_count, long int right_motor_tacho_count);
};

