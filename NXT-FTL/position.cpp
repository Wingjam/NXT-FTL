#include "position.h"

position::position(float x, float y, float direction_in_rad = 0.f) : x(x), y(y), direction_in_rad(direction_in_rad)
{
    
}

std::ostream & operator<<(std::ostream & os, const position & dt)
{
	// x, y
	os << dt.x << "," << dt.y;
	return os;
}
