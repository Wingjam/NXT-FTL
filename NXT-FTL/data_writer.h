#pragma once
#include <vector>
#include <atomic>

template<class data>
class data_writer
{
	std::vector<std::vector<data>>& data_vector;
	std::atomic<int>& currently_adding;
	int last_written_queue;
public:
	data_writer(std::vector<std::vector<data>>& _data_vector, std::atomic<int>& _currently_adding);

	// Oper need to return false when no more data will be added.
	template<class Op>
	void run(Op oper);
	~data_writer() = default;
};
