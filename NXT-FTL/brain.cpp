#include "brain.h"
using namespace nxtftl;


brain::brain(unsigned int stop_distance) : stopDistance(stop_distance)
{
}

bool brain::check_for_critical_stop(const distance_sensor_dto distance_sensor)
{
    // The distance before the robot stop
    // If distance_sensor.distance < stop_distance => stop
    return distance_sensor.distance <= stopDistance;
}

std::tuple<float, bool> brain::compute_direction(const touch_sensor_dto touch_sensor, color_sensor_dto left_color_sensor, color_sensor_dto right_color_sensor) const
{
    bool needs_to_stop = false;
    // The touch sensor stop the robot
    // If touchsensor is touch => stop
    needs_to_stop |= touch_sensor.is_pressed;

    // Calculate the direction of the robot
    const float sensor_total = static_cast<float>(left_color_sensor.value() + right_color_sensor.value());
    float direction{ left_color_sensor.value()/sensor_total - right_color_sensor.value()/sensor_total };

    return std::tuple<float, bool>{direction, needs_to_stop};
}
