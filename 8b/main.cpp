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

uint64_t steps_to_nodes_only_end_with_z(Map &map)
{
	std::vector<std::string> current_nodes = {};
	for (auto it = map.parent_children.begin(); it != map.parent_children.end(); ++it)
	{
		const auto &key = it->first;
		if (key[2] == 'A')
		{
			current_nodes.push_back(key);
		}
	}

	std::vector<uint64_t> steps = {};

	for (auto &node : current_nodes)
	{
		uint64_t node_steps = {};
		bool condition_met = false;
		while (!condition_met)
		{
			const std::pair<std::string, std::string> children = map.parent_children[node];
			const char direction = map.directions[node_steps % map.directions.length()];
			node = (direction == 'L') ? children.first : children.second;

			condition_met = (node[2] == 'Z');
			node_steps++;
		}
		steps.push_back(node_steps);
	}

	uint64_t lcm = 1;
	for (const auto s : steps)
	{
		lcm = std::lcm(lcm, s);
	}
	return lcm;
}

int main()
{
	Map map = read_input_file("input.txt");

	std::cout << steps_to_nodes_only_end_with_z(map) << std::endl;
}