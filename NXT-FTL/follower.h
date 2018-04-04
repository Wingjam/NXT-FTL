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
        buffer_manager<position>* export_buffers;

        const int EPSILON = 42;
        const int MOTOR_HIGH = 8;
        const int MOTOR_MEDIUM = 5;
        const int MOTOR_LOW = -2;

        /* Parameters */
        unsigned int size_of_internal_buffer;
        std::chrono::microseconds time_to_take_a_decision;
        std::chrono::microseconds safety_time_net;

		communication communication;
		brain brain;

        std::vector<position> internal_buffer;
        wrap_around_iterator internal_iterator;

        /* MovementHistory */
        export_to_multiple_buffers buffer_write_fct;
        movement_history move_history{};

        /* Hermite */
        wrap_around_iterator hermite_progress_iterator;
        export_to_buffers export_to_buffers_functor;
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
            std::chrono::microseconds time_to_take_a_decision,
            std::chrono::microseconds safety_time_net);
        bool Init();
		void Run();

    private:
        void send_decision_to_robot(int turn_factor);
        void execute();
        void update_all_sensor();
        bool is_distance_dangerous();
	};
}