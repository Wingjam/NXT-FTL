#pragma once
#include "position.h"
#include <functional>
#include "wrap_around_iterator.h"

namespace nxtftl
{
    class hermite
    {
    public:
        wrap_around_iterator get_points_between_subdivided(wrap_around_iterator start, wrap_around_iterator end, std::function<void(position)> buffer_write_fct, std::function<bool(void)> pred, int nb_points);

    private:
        void get_points_between(std::function<void(position)> buffer_write_fct, int nb_points, position P1, position P2);
    };
}
