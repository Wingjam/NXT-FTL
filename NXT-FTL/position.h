#pragma once
#include <limits>
#include <fstream>

class position
{
public:
    float x;
    float y;
    float direction_in_rad;

    position() = default;
    position(float, float, float = 0.f);

    bool operator==(position) const;
	friend std::ostream& operator<<(std::ostream& os, const position& dt);
};
