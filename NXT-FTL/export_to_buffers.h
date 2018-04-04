#pragma once

#include "buffer_manager.h"
#include "position.h"

class export_to_buffers
{
public:
    buffer_manager<position>* buffers;
    export_to_buffers(buffer_manager<position>* buffers) : buffers{ buffers } { }
    void operator()(position pos)
    {
        buffers->push_back(pos);
    }
};

