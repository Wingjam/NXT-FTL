#pragma once
#include "movement_history.h"
#include <tuple>

class hermite
{
public:
    void get_points_between(int, movement_history::position, movement_history::position);
private:
    std::vector<movement_history::position> positions;
};
