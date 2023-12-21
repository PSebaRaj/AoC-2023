#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct num_t
{
	int col;
	int row;
	int number;
	int order;
};

struct gear_t
{
	int numGears;
	int gearRatio;
};

int main(int argc, char *argv[])
{
	std::string input = "input.txt";
	if (argc > 1)
	{
		input = argv[1];
	}

	std::string line;
	std::fstream file(input);

	std::vector<std::string> map;
	std::vector<num_t> numbers;

	while (std::getline(file, line))
	{
		map.push_back(line);
		bool is_number = false;
		int number = -1;
		int start = 0;
		int order = 0;

		const auto add_util = [&]()
		{
			num_t n;
			n.row = map.size() - 1;
			n.col = start;
			n.number = number;
			n.order = order;
			numbers.push_back(n);
			number = -1;
			start = 0;
			order = 0;
		};

		for (int i = 0; i < line.size(); i++)
		{
			const char c = line[i];
			if (c >= '0' && c <= '9')
			{
				if (is_number == false)
				{
					number = 0;
					start = i;
					order = 0;
				}
				is_number = true;
				number = (number * 10) + int(c - '0');
				order++;
			}
			else
			{
				is_number = false;
			}

			if (number != -1 && !is_number)
			{
				add_util();
			}
		}

		if (number != -1)
		{
			add_util();
		}
	}

	std::unordered_map<int, gear_t> gears;
	for (const auto &n : numbers)
	{
		for (int row = n.row - 1; (row <= n.row + 1); row++)
		{
			if (row < 0 || row >= map.size())
				continue;

			const int col_increment = (row == n.row) ? n.order + 1 : 1;
			for (int col = n.col - 1; (col <= n.col + n.order); col += col_increment)
			{
				if (col < 0 || col >= map[row].size())
					continue;

				if (map[row][col] == '*')
				{
					const int id = map[0].size() * row + col; // assumes each row is the same length
															  //
					if (gears.find(id) == gears.end())
					{
						gear_t g;
						g.numGears = 1;
						g.gearRatio = n.number;
						gears[id] = g;
					}
					else
					{
						gears[id].numGears++;
						gears[id].gearRatio *= n.number;
					}
				}
			}
		}
	}

	int total = 0;
	for (const auto &[id, gear] : gears)
	{
		if (gear.numGears == 2)
		{
			total += gear.gearRatio;
		}
	}

	std::cout << total << std::endl;
	return 0;
}
