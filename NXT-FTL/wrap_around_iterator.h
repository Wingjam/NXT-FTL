#pragma once

#include <vector>
#include "position.h"

namespace nxtftl
{
    class wrap_around_iterator
    {
        std::vector<position>::iterator begin;
        std::vector<position>::iterator end;
        std::vector<position>::iterator iterator;
    public:
        wrap_around_iterator() = default;
        wrap_around_iterator(std::vector<position>::iterator begin, std::vector<position>::iterator end);
        wrap_around_iterator& operator++();
        position& operator*() const;
        bool operator!=(wrap_around_iterator other) const;
        wrap_around_iterator& operator+(int add);
    };
}

