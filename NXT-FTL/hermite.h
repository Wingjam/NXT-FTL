#pragma once
#include "position.h"
#include <functional>
#include "wrap_around_iterator.h"
#include <tuple>

namespace nxtftl
{
    class hermite
    {
    public:
        template <class Pred>
        std::pair<wrap_around_iterator*, int> get_points_between_subdivided(std::pair<wrap_around_iterator*, int> resume_infos, wrap_around_iterator end, std::function<void(position)> buffer_write_fct, Pred pred, int nb_points)
        {
            wrap_around_iterator* start = std::get<0>(resume_infos);
            int index = std::get<1>(resume_infos);
            for (; pred() && *start != end && start->test_if_one_ahead(end); ++(*start))
            {
                index = get_points_between(buffer_write_fct, nb_points, *(*start), start->get_next_value(), index, pred);

                if (index != nb_points) {
                    break;
                }

                index = 0;
            }

            return std::make_pair(start, index);
        }

        template <class Pred>
        int get_points_between(std::function<void(position)> buffer_write_fct, int nb_points, position P1, position P2, int initial_index, Pred pred) const
        {
            position R1 = position(cos(P1.direction_in_rad), sin(P1.direction_in_rad));
            position R2 = position(cos(P2.direction_in_rad), sin(P2.direction_in_rad));
            float inc = 1.f / nb_points;
            int index = initial_index;
            for (; index < nb_points; ++index)
            {
                if (!pred()) {
                    break;
                }

                buffer_write_fct(get_point_from_hermitian_curve(index * inc, P1, P2, R1, R2));
            }

            return index;
        }

    private:
        position get_point_from_hermitian_curve(float t, position P1, position P2, position R1, position R2) const;
    };
}
