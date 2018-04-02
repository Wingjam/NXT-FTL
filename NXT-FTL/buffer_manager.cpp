#include "buffer_manager.h"

template<class data>
bool buffer_manager<data>::increment_write_buffer() {
    int incremented_write_buffer_index = utils::increment_value(current_write_buffer_index.load(), number_of_buffers - 1);

    if (incremented_write_buffer_index == current_read_buffer_index.load()) {
        // Buffers are full, should wait but not now because only the next push cannot be added.
        return false; // did not increment
    }

    current_write_buffer_index.store(incremented_write_buffer_index);
    return true; // did increment
}

template<class data>
vector<data>* buffer_manager<data>::get_current_write_buffer() {
    return &(buffers[current_write_buffer_index.load()]);
}

template<class data>
bool buffer_manager<data>::is_read_buffer_empty() {
    return current_read_buffer_index.load() == current_write_buffer_index.load();
}

template<class data>
bool buffer_manager<data>::is_current_write_buffer_full() {
    return get_current_write_buffer()->size() == buffer_capacity;
}

template<class data>
buffer_manager<data>::buffer_manager(int number_of_buffers, int buffer_capacity) {
    this->number_of_buffers = number_of_buffers;
    this->buffer_capacity = buffer_capacity;

    for (int i = 0; i < number_of_buffers; ++i) {
        vector<data> current_buffer{};
        current_buffer.reserve(buffer_capacity);
        buffers.push_back(current_buffer);
    }
}

template<class data>
vector<data>* buffer_manager<data>::get_current_read_buffer() {
    // TODO refactor those conditions
    stage current_stage = current_adding_stage.load();
    if (current_stage == CompletlyRead) {
        return nullptr;
    }
    else if (current_stage == Adding && is_read_buffer_empty()) {
        return nullptr;
    }
    else // !is_read_buffer_empty() || current_stage == AddingCompleted
    {
        return &(buffers[current_read_buffer_index.load()]);
    }
}

template<class data>
void buffer_manager<data>::signal_current_buffer_completly_read()
{
    vector<data>* current_read_buffer = get_current_read_buffer();

    if (current_read_buffer != nullptr) {
        // reset old data vector
        current_read_buffer->clear();
        current_read_buffer->reserve(buffer_capacity);

        if (current_read_buffer_index.load() == current_write_buffer_index.load())
        {
            current_adding_stage.store(stage::CompletlyRead);
        }
        else
        {
            int next_current_read_buffer_index = utils::increment_value(current_read_buffer_index.load(), number_of_buffers - 1);
            current_read_buffer_index.store(next_current_read_buffer_index);
        }
    }
}

template<class data>
bool buffer_manager<data>::push_back(data data_to_push)
{
    if (current_adding_stage.load() != stage::Adding) {
        return false;
    }

    if (is_current_write_buffer_full()) {
        // Buffer was full last time we pushed, try to increment it again
        if (!increment_write_buffer()) {
            // buffer is full
            return false; // did not add, signal it
        }
    }

    vector<data>* current_write_buffer = get_current_write_buffer();

    current_write_buffer->push_back(data_to_push);

    if (current_write_buffer->size() == buffer_capacity) {
        increment_write_buffer();
    }

    return true;
}

template<class data>
void buffer_manager<data>::complete_adding()
{
    current_adding_stage.store(stage::AddingFinished);
}

template<class data>
bool buffer_manager<data>::is_completly_read()
{
    return current_adding_stage.load() == stage::CompletlyRead;
}


#include "position.h"
template class buffer_manager<position>;
