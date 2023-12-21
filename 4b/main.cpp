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

int copiesNumber(const std::vector<std::set<int>> &s1, const std::vector<std::set<int>> &s2, int selL, int selH)
{
	int res = 0;

	for (int s = selL; s <= selH; s++)
	{
		std::set<int> intersection;
		std::set_intersection(s1[s - 1].begin(), s1[s - 1].end(), s2[s - 1].begin(), s2[s - 1].end(),
							  std::inserter(intersection, intersection.begin()));

		res += 1;
		res += copiesNumber(s1, s2, s + 1, s + intersection.size());
	}

	return res;
}

int main()
{
	std::ifstream in("input.txt");
	std::vector<std::string> input;
	std::vector<std::set<int>> s1, s2;
	std::string line;
	int lines = 0;

	while (getline(in, line))
	{
		std::smatch m;
		regex_match(line, m, std::regex("^Card +(\\d)+: ([\\d ]+) \\| ([\\d ]+)$"));

		std::istringstream ss1(m[2]), ss2(m[3]);
		auto _s1 = std::set<int>(std::istream_iterator<int>(ss1), std::istream_iterator<int>());
		auto _s2 = std::set<int>(std::istream_iterator<int>(ss2), std::istream_iterator<int>());

		s1.emplace_back(_s1);
		s2.emplace_back(_s2);
		lines++;
	}

	in.close();

	std::cout << copiesNumber(s1, s2, 1, lines) << std::endl;
}
