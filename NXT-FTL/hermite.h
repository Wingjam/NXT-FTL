#pragma once
#include "movement_history.h"
#include <tuple>

class hermite
{
public:
    template <class ItSrc, class ItDest, class Pred>
    std::pair<ItSrc, ItDest> get_points_between_subdivided(ItSrc start, ItSrc end, ItDest dest, Pred pred, int nb_points);

private:
    template <class ItDest>
    ItDest get_points_between(ItDest dest, int nb_points, position P1, position P2);
};
