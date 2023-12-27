#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

void parse(std::set<size_t> &empty_rows, std::set<size_t> &empty_cols,
           std::vector<std::pair<size_t, size_t>> &galaxies)
{
    std::fstream file("input.txt");

    // Get width of input
    std::string line;
    std::getline(file, line);
    file.seekg(0);

    for (size_t col = 0; col < line.size(); col++)
    {
        empty_cols.insert(empty_cols.end(), col);
    }

    size_t row = 0;
    while (std::getline(file, line))
    {
        size_t col = -1;

        while ((col = line.find('#', col + 1)) != std::string::npos)
        {
            empty_cols.erase(col);
            galaxies.emplace_back(row, col);
        }
        if (galaxies.back().first != row)
        {
            empty_rows.insert(row);
        }

        row++;
    }
}

long long solve(std::set<size_t> &empty_rows, std::set<size_t> &empty_cols,
                std::vector<std::pair<size_t, size_t>> &galaxies)
{
    long long res = 0;

    for (size_t i = 0; i < galaxies.size(); i++)
    {
        for (size_t j = i + 1; j < galaxies.size(); j++)
        {
            size_t row_low = std::min(galaxies[i].first, galaxies[j].first);
            size_t row_high = std::max(galaxies[i].first, galaxies[j].first);
            size_t col_low = std::min(galaxies[i].second, galaxies[j].second);
            size_t col_high = std::max(galaxies[i].second, galaxies[j].second);

            size_t expansion = 0;

            // Count the number of empty rows in between the galaxies.
            expansion += std::count_if(empty_rows.begin(), empty_rows.end(),
                                       [row_low, row_high](size_t r)
                                       { return r > row_low && r < row_high; });

            // Count the number of empty cols in between the galaxies.
            expansion += std::count_if(empty_cols.begin(), empty_cols.end(),
                                       [col_low, col_high](size_t c)
                                       { return c > col_low && c < col_high; });

            // Calculate the distance between the galaxies, adding the (scaled) expansion
            res += row_high - row_low + col_high - col_low + expansion * 999999;
        }
    }

    return res;
}

int main()
{
    std::set<size_t> empty_rows;
    std::set<size_t> empty_cols;
    std::vector<std::pair<size_t, size_t>> galaxies;

    parse(empty_rows, empty_cols, galaxies);
    std::cout << solve(empty_rows, empty_cols, galaxies) << std::endl;

    return 0;
}
