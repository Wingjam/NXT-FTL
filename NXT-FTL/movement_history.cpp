#include "movement_history.h"

using namespace nxtftl;

movement_history::movement_history(export_to_multiple_buffers buffer_write_fct, long int initial_left_motor_tacho_count, long int initial_right_motor_tacho_count)
{
    initialize(buffer_write_fct, initial_left_motor_tacho_count, initial_right_motor_tacho_count);
}

void movement_history::initialize(export_to_multiple_buffers buffer_write_fct, long int initial_left_motor_tacho_count, long int initial_right_motor_tacho_count)
{
    this->buffer_write_fct = buffer_write_fct;
    last_snapshot = snapshot{ initial_left_motor_tacho_count, initial_right_motor_tacho_count };
    last_position = position{ 0, 0, 0 };

    buffer_write_fct(last_position);
}

position movement_history::calculate_new_position(position initial_position, snapshot initial_snapshot, snapshot destination_snapshot)
{
    float left_tacho_count_from_last_snapshot = destination_snapshot.left_motor_tacho_count - initial_snapshot.left_motor_tacho_count;
    float right_tacho_count_from_last_snapshot = destination_snapshot.right_motor_tacho_count - initial_snapshot.right_motor_tacho_count;

    float full_rotation = 360;
    float sl_distance_left_wheel = left_tacho_count_from_last_snapshot / full_rotation * WHEEL_CIRCUMFERENCE;
    float sr_distance_right_wheel = right_tacho_count_from_last_snapshot / full_rotation * WHEEL_CIRCUMFERENCE;

    position new_position{};

    if (utils::are_same_float(sl_distance_left_wheel, sr_distance_right_wheel))
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
    snapshot current_snapshot = snapshot{ left_motor_tacho_count, right_motor_tacho_count };

    position new_position = calculate_new_position(last_position, last_snapshot, current_snapshot);
    buffer_write_fct(new_position);

    last_snapshot = current_snapshot;
    last_position = new_position;
}

position movement_history::get_current_position()
{
    return last_position;
}