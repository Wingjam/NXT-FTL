#pragma once

#include "brain.h"
#include "buffer_manager.h"
#include "communication.h"
#include "movement_history.h"
#include "hermite.h"

namespace nxtftl
{
	class follower
	{
	private:
		communication communication;
		brain brain;
		buffer_manager<position>* buffers;
	public:
		follower(int stopDistance, buffer_manager<position>* buffers);
		void Run();
	};
}