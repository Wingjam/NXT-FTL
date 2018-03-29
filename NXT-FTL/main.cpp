#include "follower.h"
#include "buffer_manager.h"
#include <iostream>
#include <fstream>

using namespace nxtftl;

int main()
{
	buffer_manager<position> buffer{ 10, 200 };
	follower follow{ 10, &buffer };
	
	follow.Run();
}
