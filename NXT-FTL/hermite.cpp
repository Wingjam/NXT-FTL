#include "hermite.h"

using namespace nxtftl;

position hermite::get_point_from_hermitian_curve(float t, position P1, position P2, position R1, position R2) const
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


