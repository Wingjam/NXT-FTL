#include "utils.h"

int utils::increment_value(int initial_value, int max_value)
{
    return (initial_value + 1) % (max_value + 1);
}

int utils::distance_behind(const int my_index, const int other_index, const int max_index)
{
	int distance;
	if (other_index > my_index)
	{
		distance = other_index - my_index;
	}
	else
	{
		// distance behind the other index.
		// +1 for the 0
		distance = max_index + 1 + other_index - my_index;
	}
	return distance;
}
