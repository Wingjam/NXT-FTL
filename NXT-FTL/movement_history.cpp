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

	float full_rotation = 100;
	float sl_distance_left_wheel = left_tacho_count_from_last_snapshot / full_rotation * WHEEL_CIRCUMFERENCE;
	float sr_distance_right_wheel = right_tacho_count_from_last_snapshot / full_rotation * WHEEL_CIRCUMFERENCE;

	float O_angle_of_turn_in_rad = std::abs(sl_distance_left_wheel - sr_distance_right_wheel) / DISTANCE_BETWEEN_WHEELS;
	float r_turn_radius_left_wheel = std::fmin(sr_distance_right_wheel, sl_distance_left_wheel) / O_angle_of_turn_in_rad;

	float initial_axle_angle_rad = initial_position.direction_in_rad - (PI / 2);

	float direction_sign_multiplicator = (sl_distance_left_wheel > sr_distance_right_wheel ? 1 : -1);
	float distance_from_center_of_rotation_to_center_of_robot = r_turn_radius_left_wheel + (DISTANCE_BETWEEN_WHEELS / 2);
	float center_of_rotation_x = initial_position.x + direction_sign_multiplicator * cos(initial_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;
	float center_of_rotation_y = initial_position.y + direction_sign_multiplicator * sin(initial_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;

	float final_axle_angle_rad = initial_axle_angle_rad + (-direction_sign_multiplicator * O_angle_of_turn_in_rad);

	position new_position{};
	if (AreSame(O_angle_of_turn_in_rad, 0.0f))
	{
		new_position.direction_in_rad = initial_position.direction_in_rad;
		new_position.x = initial_position.x + cos(initial_position.direction_in_rad) * sl_distance_left_wheel;
		new_position.y = initial_position.y + sin(initial_position.direction_in_rad) * sl_distance_left_wheel;
	}
	else
	{
		new_position.direction_in_rad = final_axle_angle_rad + (PI / 2);
		new_position.x = center_of_rotation_x + sin(final_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;
		new_position.y = center_of_rotation_y + cos(final_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;
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