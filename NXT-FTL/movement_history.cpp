#include "movement_history.h"

movement_history::movement_history(long int initial_left_motor_tacho_count, long int initial_right_motor_tacho_count)
{
	snapshots = std::vector<snapshot>();
	positions = std::vector<position>();

	using namespace std::chrono;
	milliseconds now = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	snapshots.push_back({ now, initial_left_motor_tacho_count, initial_right_motor_tacho_count });
	positions.push_back({ 0, 0, 0 });
}

bool AreSame(float a, float b)
{
	return fabs(a - b) < 0.000001;
}

position movement_history::calculate_new_position(position initial_position, snapshot initial_snapshot, snapshot destination_snapshot)
{
	float left_tacho_count_from_last_snapshot = destination_snapshot.left_motor_tacho_count - initial_snapshot.left_motor_tacho_count;
	float right_tacho_count_from_last_snapshot = destination_snapshot.right_motor_tacho_count - initial_snapshot.right_motor_tacho_count;

	float full_rotation = 360;
	float sl_distance_left_wheel = left_tacho_count_from_last_snapshot / full_rotation * WHEEL_CIRCUMFERENCE;
	float sr_distance_right_wheel = right_tacho_count_from_last_snapshot / full_rotation * WHEEL_CIRCUMFERENCE;

	position new_position{};

	if (AreSame(sl_distance_left_wheel, sr_distance_right_wheel))
	{
		new_position.direction_in_rad = initial_position.direction_in_rad;
		new_position.x = initial_position.x + cos(initial_position.direction_in_rad) * sl_distance_left_wheel;
		new_position.y = initial_position.y + sin(initial_position.direction_in_rad) * sl_distance_left_wheel;
	}
	else
	{
		new_position.direction_in_rad = (sr_distance_right_wheel - sl_distance_left_wheel) / DISTANCE_BETWEEN_WHEELS + initial_position.direction_in_rad;
		new_position.x = initial_position.x + (DISTANCE_BETWEEN_WHEELS * (sr_distance_right_wheel + sl_distance_left_wheel)) / (2 * (sr_distance_right_wheel - sl_distance_left_wheel))
			* (sin((sr_distance_right_wheel - sl_distance_left_wheel) / DISTANCE_BETWEEN_WHEELS + initial_position.direction_in_rad) - sin(initial_position.direction_in_rad));
		new_position.y = initial_position.y - (DISTANCE_BETWEEN_WHEELS * (sr_distance_right_wheel + sl_distance_left_wheel)) / (2 * (sr_distance_right_wheel - sl_distance_left_wheel))
			* (cos((sr_distance_right_wheel - sl_distance_left_wheel) / DISTANCE_BETWEEN_WHEELS + initial_position.direction_in_rad) - cos(initial_position.direction_in_rad));
	}
	return new_position;
}

void movement_history::log_rotation(long int left_motor_tacho_count, long int right_motor_tacho_count)
{
	using namespace std::chrono;
	milliseconds now = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	snapshots.push_back({ now, left_motor_tacho_count, right_motor_tacho_count });
	position new_position = calculate_new_position(positions[positions.size() - 1], snapshots[snapshots.size() - 2], snapshots[snapshots.size() - 1]);
	positions.push_back(new_position);
}

position movement_history::get_current_position()
{
	return positions[positions.size() - 1];
}

std::vector<position> movement_history::get_positions()
{
	return positions;
}

void movement_history::write_positions_to_stream(std::ostream& stream)
{
	for (int i = 0; i < positions.size(); ++i)
	{
		stream << positions[i] << std::endl;
	}
}