#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <regex>

using Race = std::pair<long, long>;

inline auto distance(long buttonCharge, long time)
{
	return buttonCharge * time;
}

int win_race(Race race)
{
	long totalTime = race.first;
	long recordDistance = race.second;
	long count = 0;

	for (long chargeTime = 0; chargeTime < totalTime; chargeTime++)
	{
		count += distance(chargeTime, totalTime - chargeTime) > recordDistance ? 1 : 0;
	}

	return count;
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

	std::string time, dist, _time, _dist;
	while (sstream_time >> _time and sstream_dist >> _dist)
	{
		time += _time;
		dist += _dist;
	}

	std::cout << win_race(std::make_pair(std::stol(time), std::stol(dist))) << std::endl;
}
