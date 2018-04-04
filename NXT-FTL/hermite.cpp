#include "hermite.h"

using namespace nxtftl;

position get_point_from_hermitian_curve(float t, position P1, position P2, position R1, position R2)
{
    // Hermitian curves
    //                      [ 2 -2  1  1] [P1]
    // Q(t) = [t^3 t^2 t 1] [-3  3 -2 -1] [P2]
    //                      [ 0  0  1  0] [R1]
    //                      [ 1  0  0  0] [R2]
    //
    //        [2 * t^3 - 3 * t^2 + 1] [P1]
    // Q(t) = [-2 *t^3 + 3 * t^2    ] [P2]
    //        [t^3 - 2 * t^2 + 1    ] [R1]
    //        [t^3 - t^2            ] [R2]

    float t2 = pow(t, 2);
    float t3 = pow(t, 3);

    float f1 = 2 * t3 - 3 * t2 + 1;
    float f2 = -2 * t3 + 3 * t2;
    float f3 = t3 - 2 * t2 + t;
    float f4 = t3 - t2;

    float Px = f1 * P1.x + f2 * P2.x + f3 * R1.x + f4 * R2.x;
    float Py = f1 * P1.y + f2 * P2.y + f3 * R1.y + f4 * R2.y;

    return position(Px , Py);
}

wrap_around_iterator hermite::get_points_between_subdivided(wrap_around_iterator start, wrap_around_iterator end, std::function<void(position)> buffer_write_fct, std::function<bool(void)> pred, int nb_points)
{
    for (; start != end && (start + 1) != end && pred(); ++start)
    {
        get_points_between(buffer_write_fct, nb_points, *start, *(start + 1));
    }

    return start;
}

void hermite::get_points_between(std::function<void(position)> buffer_write_fct, int nb_points, position P1, position P2)
{
    position R1 = position(cos(P1.direction_in_rad), sin(P1.direction_in_rad));
    position R2 = position(cos(P2.direction_in_rad), sin(P2.direction_in_rad));
    float inc = 1.f / nb_points;

    for (float t = 0.f; t < 1.f; t += inc)
    {
        auto res = get_point_from_hermitian_curve(t, P1, P2, R1, R2);
        buffer_write_fct(res);
    }
}