#include "wrap_around_iterator.h"

using namespace nxtftl;

wrap_around_iterator::wrap_around_iterator(std::vector<position>::iterator begin, std::vector<position>::iterator end)
{
    this->begin = begin;
    this->end = end;
    iterator = begin;
}

wrap_around_iterator& wrap_around_iterator::operator++()
{
    ++iterator;
    if (iterator == end) {
        iterator = begin;
    }

    return *this;
}

position& wrap_around_iterator::operator*() const
{
    return *iterator;
}

bool wrap_around_iterator::operator!=(wrap_around_iterator other) const
{
    return iterator != other.iterator;
}

wrap_around_iterator& wrap_around_iterator::operator+(int add)
{
    auto new_iterator = iterator + add;
    while (new_iterator >= end) {
        new_iterator = begin + (new_iterator - end);
    }

    iterator = new_iterator;

    return *this;
}
