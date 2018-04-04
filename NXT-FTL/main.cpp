#include "follower.h"
#include "buffer_manager.h"
#include <thread>
#include <atomic>
#include "data_writer.h"

using namespace nxtftl;

int main()
{
	buffer_manager<position> buffer{ 10, 100 };
	follower follow{ 10, &buffer };
    data_writer writer{ &buffer };

    std::thread data_writer_thread([&writer]() {
        writer.run();
    });
	
	follow.Run();

    buffer.complete_adding();

    data_writer_thread.join();

    std::getchar();
}
