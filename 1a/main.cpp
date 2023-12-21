#include <cstdint>
#include <fstream>
#include <string>

int find_calibration_value(std::string line)
{
	std::string first(1, line[line.find_first_of("123456789")]);
	std::string last(1, line[line.find_last_of("0123456789")]);

	std::size_t value = std::stoi(first + last);
	return value;
}

int main()
{
	std::ifstream file("input.txt");
	std::int32_t sum = 0;
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			sum += find_calibration_value(line);
		}
		file.close();
	}
	printf("Result: %d\n", sum);
}
