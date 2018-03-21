#include "follower.h"
#include <iostream>
#include <fstream>

using namespace nxtftl;

int main()
{
	follower follow{ 10 };
	follow.Run();

	ofstream myfile;
	myfile.open("output.txt");
	std::vector<movement_history::position> positions = movement_history.get_positions();
	for each (movement_history::position position in positions)
	{
		myfile << "(" << position.x << "," << position.y << ")";
	}
	myfile.close();
}
