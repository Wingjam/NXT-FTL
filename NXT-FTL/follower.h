#pragma once

#include "brain.h"
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
	public:
		follower(int stopDistance);
		void Run();
	};
}