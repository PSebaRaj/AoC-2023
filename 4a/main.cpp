#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <regex>
#include <numeric>
#include <cmath>

int main()
{
	std::ifstream in("input.txt");
	std::vector<std::string> input;
	std::string line;
	int res = 0;

	while (getline(in, line))
	{
		std::smatch m;
		regex_match(line, m, std::regex("^Card +(\\d)+: ([\\d ]+) \\| ([\\d ]+)$"));

		std::istringstream ss1(m[2]), ss2(m[3]);

		auto s1 = std::set<int>(std::istream_iterator<int>(ss1), std::istream_iterator<int>());
		auto s2 = std::set<int>(std::istream_iterator<int>(ss2), std::istream_iterator<int>());

		std::set<int> intersection;
		std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
							  std::inserter(intersection, intersection.begin()));

		if (not intersection.empty())
			res += pow(2, intersection.size() - 1);
	}

	in.close();

	std::cout << res << std::endl;
}
