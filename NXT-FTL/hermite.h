#pragma once
#include "movement_history.h"
#include <tuple>

class hermite
{
public:
    void get_points_between(int, position, position);
private:
    std::vector<position> positions;
};
