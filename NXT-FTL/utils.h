#pragma once
class utils
{
private:
	utils() = default;
	~utils() = default;
public:
	static int increment_value(int initial_value, int max_value);
	static int distance_behind(const int my_index, const int other_index, const int max_index);
};

