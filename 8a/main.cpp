#include <fstream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct Map
{
	std::string directions;
	std::map<std::string, std::pair<std::string, std::string>> parent_children;
};

Map read_input_file(std::string input_file)
{
	std::string directions;
	std::map<std::string, std::pair<std::string, std::string>> parent_children;

	std::ifstream file_input;
	file_input.open(input_file);
	if (file_input.is_open())
	{
		std::getline(file_input, directions);
		for (std::string line; std::getline(file_input, line);)
		{
			if (line.length() > 0)
			{
				std::string key = line.substr(0, line.find_first_of(' '));
				std::string left = line.substr(line.find_first_of('(') + 1, 3);
				std::string right = line.substr(line.find_last_of(')') - 3, 3);

				std::pair<std::string, std::string> branches = {left, right};
				parent_children[key] = branches;
			}
		}
		file_input.close();
	}
	return {directions, parent_children};
}

uint64_t steps_to_reach_zzz(Map &map)
{
	std::string current_node = "AAA";
	const std::string end_node = "ZZZ";
	uint64_t steps = 0;

	while (current_node != end_node)
	{
		const std::pair<std::string, std::string> current_children = map.parent_children[current_node];
		const char direction = map.directions[steps % map.directions.length()];
		current_node = (direction == 'L') ? current_children.first : current_children.second;
		steps++;
	}
	return steps;
}

int main()
{
	Map map = read_input_file("input.txt");

	std::cout << steps_to_reach_zzz(map) << std::endl;
}