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

    atomic<bool> adding_completed{ false };
    atomic<int> current_write_buffer_index = 0;
    atomic<int> current_read_buffer_index = 0;

    bool increment_write_buffer();
    vector<data>* get_current_write_buffer();
    bool is_read_buffer_empty();
    bool is_current_write_buffer_full();

public:
    buffer_manager(int number_of_buffers, int buffer_capacity);

    vector<data>* get_current_read_buffer();
    void signal_current_buffer_completly_read();
    bool push_back(data data_to_push);
    void complete_adding();
};
