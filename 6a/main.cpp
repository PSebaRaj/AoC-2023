#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>

using Race = std::pair<int, int>;

inline auto distance(int buttonCharge, int time)
{
	return buttonCharge * time;
}

int win_race(std::vector<Race> races)
{
	int res = 1;

	for (const auto &[totalTime, recordDistance] : races)
	{
		int count = 0;
		for (int chargeTime = 0; chargeTime < totalTime; chargeTime++)
		{
			count += distance(chargeTime, totalTime - chargeTime) > recordDistance ? 1 : 0;
		}
		res *= count;
	}

	return res;
}

int main()
{
	std::ifstream in("input.txt");

	std::string time_buf;
	std::smatch sm_time;
	getline(in, time_buf);
	bool ok1 = regex_match(time_buf, sm_time, std::regex("[A-Za-z]+:([\\d+ ]+)"));
	std::stringstream sstream_time(sm_time.str(1));

	std::string dist_buf;
	std::smatch sm_dist;
	getline(in, dist_buf);
	bool ok2 = regex_match(dist_buf, sm_dist, std::regex("[A-Za-z]+:([\\d+ ]+)"));
	std::stringstream sstream_dist(sm_dist.str(1));

	in.close();

	if (not ok1 || not ok2)
	{
		return 1;
	}

	std::vector<Race> v;
	int time, dist;
	while (sstream_time >> time and sstream_dist >> dist)
	{
		v.emplace_back(std::make_pair(time, dist));
	}

	std::cout << win_race(v) << std::endl;
}
