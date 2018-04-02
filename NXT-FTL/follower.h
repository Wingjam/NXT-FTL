#pragma once

#include "brain.h"
#include "buffer_manager.h"
#include "communication.h"
#include "movement_history.h"
#include "hermite.h"
#include <vector>

namespace nxtftl
{
	class follower
	{
        const int SIZE_OF_INTERNAL_BUFFER = 10;
		communication communication;
		brain brain;
        std::vector<position> internal_buffer{};
        buffer_manager<position>* export_buffers;
	public:
		follower(int stopDistance, buffer_manager<position>* export_buffers);
		void Run();
	};
}