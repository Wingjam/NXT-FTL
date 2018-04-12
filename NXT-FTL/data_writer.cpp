#include "data_writer.h"
#include "utils.h"
#include <fstream>
#include <algorithm>

data_writer::data_writer(buffer_manager<position>* buffers, string filename)
{
    this->buffers = buffers;
    this->filename = filename;
}

void data_writer::run()
{
    std::ofstream my_file;
    my_file.open(filename);
    while (!buffers->is_completly_read())
    {
        std::vector<position>* current_read_buffer = buffers->get_current_read_buffer();

        if (current_read_buffer == nullptr) {
            continue;
        }

        std::for_each(std::begin(*current_read_buffer), std::end(*current_read_buffer), [&my_file](position pos) {
            my_file << pos << std::endl;
        });

        buffers->signal_current_buffer_completly_read();
    }
    my_file.close();
}
