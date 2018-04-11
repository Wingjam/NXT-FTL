#include "follower.h"
#include "buffer_manager.h"
#include <thread>
#include <atomic>
#include "data_writer.h"

using namespace nxtftl;

int main()
{
    int stopDistance = 10;
    unsigned int internal_buffer_size = 100;
    unsigned int number_of_points_in_curve = 10000;
    buffer_manager<position> buffers{ 500, 1000 };
    follower follow{ &buffers, stopDistance, internal_buffer_size, number_of_points_in_curve };
    data_writer writer{ &buffers };

    std::thread data_writer_thread([&writer]() {
        writer.run();
    });
    
    follow.Init();
    follow.Run();

    buffers.complete_adding();

    data_writer_thread.join();

    std::getchar();
}
