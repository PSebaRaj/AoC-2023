#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

long long comb(int n, int k)
{
	if (k == 0 || k == n)
	{
		return 1;
	}
	return comb(n - 1, k - 1) + comb(n - 1, k);
}

// Lagrange
long long sum_prev_values(const std::vector<int> &nums)
{
	int n = nums.size();
	long long res = 0;
	for (int i = 0; i < n; ++i)
	{
		res += nums[i] * comb(n, i + 1) * pow(-1, i);
	}
	return res;
}

int main()
{
	std::ifstream inputFile("input.txt");

	long long res = 0;
	std::string line;
	while (getline(inputFile, line))
	{
		std::vector<int> nums;
		std::stringstream ss(line);
		int num;

		while (ss >> num)
		{
			nums.push_back(num);
		}
		res += sum_prev_values(nums);
	}

	std::cout << res << std::endl;
}
