#pragma once

#include "brain.h"
#include "buffer_manager.h"
#include "communication.h"
#include "movement_history.h"
#include "export_to_buffers.h"
#include "export_to_multiple_buffers.h"
#include "hermite.h"
#include <vector>
#include <chrono>

namespace nxtftl
{
    class wrap_around_iterator;
    class hermite;

    class follower
    {
        /* Constants */
        const int MOTOR_HIGH = 9;
        const int MOTOR_MEDIUM = 6;
        const float TURN_MULTIPLICATOR = 4.0f;

        /* Parameters */
        unsigned int size_of_internal_buffer;
        unsigned int number_of_points_between_positions;

        /* Decision making logic and communication with other threads */
        buffer_manager<position>* export_buffers;
        communication communication;
        brain brain;

        /* Internal data structures and data structures accessors */
        std::vector<position> internal_buffer;
        wrap_around_iterator internal_iterator_writer;
        wrap_around_iterator internal_iterator_reader;
        std::pair<wrap_around_iterator*, int> hermite_progress;
        export_to_multiple_buffers buffer_write_fct;
        export_to_buffers export_to_buffers_functor;

        /* Position reporting logic */
        movement_history move_history{};
        hermite hermite{};

        /* DTOs*/
        motor_dto leftMotor{};
        motor_dto rightMotor{};
        touch_sensor_dto touchSensor{};
        color_sensor_dto leftColorSensor{};
        color_sensor_dto rightColorSensor{};
        distance_sensor_dto distanceSensor{};

    public:
        follower(buffer_manager<position>* export_buffers,
            int stopDistance,
            unsigned int size_of_internal_buffer,
            unsigned int number_of_points_between_positions);
        bool Init();
        void Run();

    private:
        void send_decision_to_robot(float turn_factor);
        void execute();
        void update_all_sensor();
        bool evaluate_distance();
    };
}