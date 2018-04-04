#pragma once
#include "position.h"
#include <functional>

namespace nxtftl
{
    class hermite
    {
    public:
        template <class ItSrc, class Pred>
        ItSrc get_points_between_subdivided(ItSrc start, ItSrc end, std::function<void(position)> buffer_write_fct, Pred pred, int nb_points);

    private:
        void get_points_between(std::function<void(position)> buffer_write_fct, int nb_points, position P1, position P2);
    };
}
