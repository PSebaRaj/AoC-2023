#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct num_t {
	int col;
	int row;
	int number;
	int order;
};

int main(int argc, char * argv[]) {
	std::string input = "input.txt";
	if (argc > 1) {
		input = argv[1];
	}

	std::string line;
	std::fstream file(input);

	std::vector<std::string> map;
	std::vector<num_t> numbers;

	while(std::getline(file, line)) {
		map.push_back(line);
		bool is_number = false;
		int number = -1;
		int start = 0;
		int order = 0;

		const auto add_util = [&]() {
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

		for (int i = 0; i < line.size(); i++) {
			const char c = line[i];
			if (c >= '0' && c <= '9') {
				if (is_number == false) {
					number = 0;
					start = i;
					order = 0;
				}
				is_number = true;
				number = (number * 10) + int (c - '0');
				order++;
			} else {
				is_number = false;
			}

			if (number != -1 && !is_number) {
				add_util();
			}
		}

		if (number != -1) {
			add_util();
		}
	}

	// need to refactor lol
	std::vector<int> parts;
	for (const auto& n : numbers) {
		bool still_searching = true;

		for (int row = n.row - 1; (row <= n.row + 1) && still_searching; row++) {
			if (row < 0 || row >= map.size()) continue;

			const int col_increment = (row == n.row) ? n.order + 1 : 1;
				for (int col = n.col - 1; (col <= n.col + n.order) && still_searching; col += col_increment) {
					if (col < 0 || col >= map[row].size()) continue;
					if (map[row][col] != '.' && !(map[row][col] >= '0' && map[row][col] <= '9')) {
						still_searching = false;
						parts.push_back(n.number);
					}
				}
		}
	}

	const auto total = std::accumulate(std::begin(parts), std::end(parts), 0);
	std::cout << total << std::endl;

	return 0;
}
