#include "movement_history.h"

movement_history::movement_history()
{
}


movement_history::~movement_history()
{
}

void movement_history::log_rotation(long int left_motor_tacho_count, long int right_motor_tacho_count)
{
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	snapshots.push_back({ ms, left_motor_tacho_count, right_motor_tacho_count });
}