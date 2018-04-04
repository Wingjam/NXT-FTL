#pragma once

#include "wrap_around_iterator.h"
#include "buffer_manager.h"
#include "position.h"

namespace nxtftl
{
    class export_to_multiple_buffers
    {
    public:
        buffer_manager<position>* buffers;
        wrap_around_iterator* internal_iterator;
        export_to_multiple_buffers() = default;
        export_to_multiple_buffers(buffer_manager<position>* buffers, wrap_around_iterator* internal_iterator)
            : buffers{ buffers }, internal_iterator{ internal_iterator }
        { }

        void operator()(position pos)
        {
            buffers->push_back(pos);

            *(*internal_iterator) = pos;
            ++(*internal_iterator);
        }
    };
}
