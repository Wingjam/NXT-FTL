#pragma once

class position
{
public:
    float x;
    float y;
    float direction_in_rad;
    position(float, float, float = 0.f);
};
