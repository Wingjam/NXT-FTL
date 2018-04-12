#include "position.h"
#include "utils.h"

position::position(float x, float y, float direction_in_rad  /* = 0.f */) : x(x), y(y), direction_in_rad(direction_in_rad)
{
    
}

bool position::operator==(position other) const {
    return
        utils::are_same_float(this->x, other.x) &&
        utils::are_same_float(this->y, other.y) &&
        utils::are_same_float(this->direction_in_rad, other.direction_in_rad);
}
std::ostream & operator<<(std::ostream & os, const position & dt)
{
    // x, y
    os << dt.x << "," << dt.y;
    return os;
}

