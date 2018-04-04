#pragma once
#include "position.h"
#include <functional>
#include "wrap_around_iterator.h"

namespace nxtftl
{
    class hermite
    {
    public:
        template <class Pred>
        wrap_around_iterator get_points_between_subdivided(wrap_around_iterator start, wrap_around_iterator end, std::function<void(position)> buffer_write_fct, Pred pred, int nb_points)
        {
            for (; pred() && start != end && start.test_if_one_ahead(end); ++start)
            {
                get_points_between(buffer_write_fct, nb_points, *start, start.get_next_value());
            }

            return start;
        }

    private:
        void get_points_between(std::function<void(position)> buffer_write_fct, int nb_points, position P1, position P2);
    };
}
