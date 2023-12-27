#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>

struct Key
{
    std::size_t reconstructed_size;
    std::size_t duplicated_idx;

    Key(const std::size_t reconstructed_size,
        const std::size_t duplicated_idx)
        : reconstructed_size(reconstructed_size), duplicated_idx(duplicated_idx) {}

    bool operator==(const Key &k) const
    {
        return reconstructed_size == k.reconstructed_size && duplicated_idx == k.duplicated_idx;
    }
};

struct hasher
{
    std::size_t operator()(const Key &p) const
    {
        return p.reconstructed_size * p.duplicated_idx;
    }
};

struct Record
{
    std::string damaged;
    std::vector<std::size_t> duplicated;
};

bool compare(const std::string &damaged, const std::vector<char> &reconstructed)
{
    for (std::size_t i = 0; i < reconstructed.size(); i++)
    {
        if (damaged[i] != '?' && reconstructed[i] != damaged[i])
        {
            return false;
        }
    }
    return true;
}

std::vector<std::size_t> extract_numbers(const std::string &s)
{
    std::vector<std::size_t> numbers;
    std::size_t start = 0;
    std::size_t end = s.find(',', start);

    while (end != std::string::npos)
    {
        numbers.emplace_back(std::stoi(s.substr(start, end - start)));
        start = end + 1;
        end = s.find(',', start);
    }

    numbers.emplace_back(std::stoi(s.substr(start, s.size() - start)));
    return numbers;
}

bool compare_with_duplicated(const std::vector<char> &reconstructed, const std::vector<std::size_t> &duplicated)
{
    std::size_t idx_duplicated = 0;
    std::size_t idx_reconstructed = 0;

    while (idx_reconstructed < reconstructed.size())
    {
        if (reconstructed[idx_reconstructed] == '#')
        {
            for (std::size_t j = 0; j < duplicated[idx_duplicated]; j++)
            {
                if (reconstructed[idx_reconstructed] != '#')
                {
                    return false;
                }
                idx_reconstructed++;
            }
            idx_duplicated++;
            if (reconstructed[idx_reconstructed] != '.')
            {
                return false;
            }
        }
        idx_reconstructed++;
    }

    return true;
}

void dp(const std::string &damaged, std::vector<char> &reconstructed,
        const std::vector<std::size_t> &duplicated, std::size_t &count,
        const std::size_t duplicated_idx, std::unordered_map<Key, std::size_t, hasher> &seen)
{
    // Does not match pattern in damaged record (note compare matches upto the length of reconstructed only)
    if (!compare(damaged, reconstructed))
    {
        return;
    }

    // Reconstructed pattern does not duplicate record
    if (!compare_with_duplicated(reconstructed, duplicated))
    {
        return;
    }

    // If the reconstructed record and the damage record ahve the same size and are equal
    if (damaged.size() == reconstructed.size() && compare(damaged, reconstructed))
    {
        if (duplicated_idx == duplicated.size())
        {
            count++;
        }
        return;
    }

    if (std::begin(duplicated) + duplicated_idx <= std::end(duplicated) && ((std::accumulate(std::begin(duplicated) + duplicated_idx, std::end(duplicated), 0) + duplicated.size() - duplicated_idx) > (damaged.size() - reconstructed.size())))
    {
        return;
    }

    const std::size_t count_on_entry = count; // count on entry into dp function

    const auto &dp_wrapper_with_cache_check = [&damaged, &reconstructed, &duplicated, &count, &seen](const std::size_t duplicated_idx)
    {
        if (const auto it = seen.find(Key(reconstructed.size(), duplicated_idx)); it != seen.end())
        {
            count += it->second;
        }
        else
        {
            dp(damaged, reconstructed, duplicated, count, duplicated_idx, seen);
        }
    };

    const std::size_t idx = reconstructed.size();
    if (damaged[idx] == '?')
    { // '?' at idx
        // Assume the '?' == '.'
        {
            reconstructed.push_back('.');
            dp_wrapper_with_cache_check(duplicated_idx);
            reconstructed.pop_back();
        }
        // Assume the '?' == '#'
        {
            if (duplicated_idx < duplicated.size())
            {
                for (std::size_t j = 0; j < duplicated[duplicated_idx]; j++)
                {
                    reconstructed.push_back('#');
                }
                reconstructed.push_back('.'); // There is always a '.' after a group of damaged springs
                dp_wrapper_with_cache_check(duplicated_idx + 1);
            }
        }
    }
    else if (damaged[idx] == '.')
    {
        reconstructed.push_back(damaged[idx]);
        dp_wrapper_with_cache_check(duplicated_idx);
    }
    else if (duplicated_idx < duplicated.size())
    {
        for (std::size_t j = 0; j < duplicated[duplicated_idx]; j++)
        {
            reconstructed.push_back('#');
        }
        reconstructed.push_back('.'); // There is always a '.' after a group of damaged springs
        dp_wrapper_with_cache_check(duplicated_idx + 1);
    }
    while (reconstructed.size() > idx)
    {
        reconstructed.pop_back();
    }
    seen[Key(reconstructed.size(), duplicated_idx)] = count - count_on_entry; // Update cache
    return;
}

Record parse(const std::string &line)
{
    const auto separator = line.find(' ');
    Record record;
    record.damaged = line.substr(0, separator);
    record.duplicated = extract_numbers(line.substr(separator + 1, line.size() - separator - 1));
    return record;
}

int main(int argc, char *argv[])
{
    std::string line;
    std::fstream file("input.txt");
    std::vector<Record> records;

    while (std::getline(file, line))
    {
        const auto folded_record = parse(line);

        Record unfolded_record;
        unfolded_record.damaged.append(folded_record.damaged);
        unfolded_record.damaged.push_back('.'); // Add a '.' to ensure that the last value in the input is a '.'
        unfolded_record.duplicated.insert(std::end(unfolded_record.duplicated), std::begin(folded_record.duplicated), std::end(folded_record.duplicated));
        records.emplace_back(unfolded_record);
    }

    long long res = 0;
    for (const auto &record : records)
    {
        std::size_t count = 0;
        std::vector<char> reconstructed;
        std::unordered_map<Key, std::size_t, hasher> seen;

        dp(record.damaged, reconstructed, record.duplicated, count, 0, seen);
        res += count;
    }

    std::cout << res << std::endl;

    return 0;
}