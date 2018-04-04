#pragma once
#include "position.h"
#include <functional>
#include "wrap_around_iterator.h"

namespace nxtftl
{
    class hermite
    {
    public:
        template <class ItSrc, class Pred>
        ItSrc get_points_between_subdivided(ItSrc start, ItSrc end, std::function<void(position)> buffer_write_fct, Pred pred, int nb_points)
        {
            for (; start != end && (start + 1) != end && pred(); ++start)
            {
                get_points_between(buffer_write_fct, nb_points, *start, *(start + 1));
            }

            return start;
        }

    private:
        void get_points_between(std::function<void(position)> buffer_write_fct, int nb_points, position P1, position P2);
    };
}
