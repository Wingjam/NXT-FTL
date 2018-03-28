#pragma once
#include <fstream>

class position
{
public:
    float x;
    float y;
    float direction_in_rad;
    position(float, float, float);
	position() = default;
	friend std::ostream& operator<<(std::ostream& os, const position& dt);
};
