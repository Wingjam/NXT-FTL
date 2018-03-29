#pragma once

#include <vector>
#include <atomic>
#include "utils.h"

using namespace std;

template<class data>
class buffer_manager
{
	int number_of_buffers = 0;
	int buffer_capacity = 0;

	vector<vector<data>> buffers;

	atomic<int> current_write_buffer_index = 0;
	atomic<int> current_read_buffer_index = 0;

	bool increment_write_buffer() {
		int incremented_write_buffer_index = utils::increment_value(current_write_buffer_index.load(), number_of_buffers);

		if (incremented_write_buffer_index == current_read_buffer_index.load()) {
			// Buffers are full, should wait but not now because only the next push cannot be added.
			return false; // did not increment
		}

		current_write_buffer_index.store(incremented_write_buffer_index);
		return true; // did increment
	}

	vector<data> get_current_write_buffer() {
		return buffers[current_write_buffer_index.load()];
	}

	bool is_read_buffer_empty() {
		return current_read_buffer_index.load() == current_write_buffer_index.load();
	}

	bool is_current_write_buffer_full() {
		return get_current_write_buffer().size() == buffer_capacity;
	}

public:
	buffer_manager() = default;
	buffer_manager(int number_of_buffers, int buffer_capacity) {
		this->number_of_buffers = number_of_buffers;
		this->buffer_capacity = buffer_capacity;

		for (int i = 0; i < number_of_buffers; ++i) {
			vector<data> current_buffer{};
			current_buffer.reserve(buffer_capacity);
			buffers.push_back(current_buffer);
		}
	}
	~buffer_manager() = default;

	vector<data> get_current_read_buffer() {
		if (is_read_buffer_empty()) {
			return null; // No read buffer available
		}
		return buffers[current_read_buffer_index.load()];
	}

	void signal_current_buffer_completly_read()
	{
		if (!is_read_buffer_empty()) {
			// reset old data vector
			get_current_read_buffer().clear();
			get_current_read_buffer().reserve(buffer_capacity);

			int next_current_read_buffer_index = utils::increment_value(current_read_buffer_index.load(), number_of_buffers);
			current_read_buffer_index.store(next_current_read_buffer_index);
		}
	}

	bool push_back(data data)
	{
		if (is_current_write_buffer_full()) {
			// Buffer was full last time we pushed, try to increment it again
			if (!increment_write_buffer()) {
				// buffer is full
				return false; // did not add, signal it
			}
		}

		vector<data> current_write_buffer = get_current_write_buffer();

		current_write_buffer.push_back(data);
		
		if (current_write_buffer.size() == buffer_capacity) {
			increment_write_buffer();
		}

		return true;
	}
};

