#include "data_writer.h"
#include "utils.h"
#include <fstream>

template<class data>
data_writer<data>::data_writer(std::vector<std::vector<data>>& _data_vector, std::atomic<int>& _currently_adding)
	: data_vector(_data_vector), currently_adding(_currently_adding), last_written_queue(-1)
{
}

template <class data>
template <class Op>
void data_writer<data>::run(Op oper)
{
	std::ofstream myfile;
	myfile.open("output.txt");
	const int number_of_queue = data_vector.size();
	while (oper())
	{
		// We need to stay one queue behind
		while (utils::distance_behind(last_written_queue, currently_adding, number_of_queue - 1) > 1)
		{
			last_written_queue = utils::increment_value(last_written_queue, number_of_queue);
			std::vector<data> current_data = data_vector[last_written_queue];
			for (auto && data_to_write : current_data)
			{
				myfile << data_to_write << std::endl;
			}
		}
	}
	myfile.close();
}
