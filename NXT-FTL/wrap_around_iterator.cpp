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

bool wrap_around_iterator::test_if_one_ahead(wrap_around_iterator end_wrap_around)
{
    bool something_ahead;
    iterator++;

    if (iterator == end)
    {
        something_ahead = begin != end_wrap_around.iterator;
    }
    else
    {
        something_ahead = iterator != end_wrap_around.iterator;
    }

    iterator--;
    return something_ahead;
}

position& wrap_around_iterator::get_next_value()
{
    ++iterator;
    if (iterator == end)
    {
        --iterator;
        return *begin;
    }
    else
    {
        return *(iterator--);
    }
}
