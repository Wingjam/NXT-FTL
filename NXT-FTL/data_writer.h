#pragma once
#include <vector>
#include <atomic>
#include "buffer_manager.h"
#include "position.h"

class data_writer
{
    buffer_manager<position>* buffers;
public:
	data_writer(buffer_manager<position>* buffers);
	void run();
	~data_writer() = default;
};
