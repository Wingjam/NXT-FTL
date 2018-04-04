#include "wrap_around_iterator.h"

using namespace nxtftl;

wrap_around_iterator::wrap_around_iterator(std::vector<position>::iterator begin, std::vector<position>::iterator end)
{
    this->begin = begin;
    this->end = end;
    iterator = begin;
}

wrap_around_iterator::wrap_around_iterator(std::vector<position>::iterator begin, std::vector<position>::iterator end, std::vector<position>::iterator& iterator)
{
    this->begin = begin;
    this->end = end;
    this->iterator = iterator;
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

bool wrap_around_iterator::test_if_one_ahead(std::vector<position>::iterator end_iterator)
{
    bool something_ahead;
    iterator++;

    if (iterator == end) {
        something_ahead = begin == end_iterator;
    }
    else
    {
        something_ahead = iterator == end_iterator;
    }

    iterator--;
    return something_ahead;
}

wrap_around_iterator& wrap_around_iterator::operator+(std::vector<position>::difference_type add) const
{
    std::vector<position>::iterator new_iterator{ iterator };
    //new_iterator += add;
    while (new_iterator >= end) {
        new_iterator = begin + (new_iterator - end);
    }
    return wrap_around_iterator{ begin, end, new_iterator };
}
