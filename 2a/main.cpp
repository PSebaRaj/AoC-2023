#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>

struct bag {
    bag(int red = 0, int green = 0, int blue = 0)
        : red_cubes(red), green_cubes(green), blue_cubes(blue) {}

	// b/c we have replacement
    void update(int red, int green, int blue) {
        if (red_cubes < red) {
            red_cubes = red;
        }
        if (green_cubes < green) {
            green_cubes = green;
        }
        if (blue_cubes < blue) {
            blue_cubes = blue;
        }
    }

    int red_cubes;
    int green_cubes;
    int blue_cubes;
};

static const bag MAX_BAG(12,13,14);

bool valid_bag(const bag& lhs, const bag& rhs);
void parse_bag(const std::string& round, bag& source);

int main() {
    std::string game;
    std::string round;
    int game_id = 1;
    int sum = 0;
    std::ifstream infile("input.txt");

    while (std::getline(infile,game)) {
        bag round_bag;
        std::istringstream game_stream(game);
        game_stream.ignore(255, ':');

        while (std::getline(game_stream, round, ';')) {
            parse_bag(round, round_bag);
        }
        if (valid_bag(MAX_BAG, round_bag)) {
            sum += game_id;
        }
        game_id++;
    }

    std::cout << "Sum of valid game ids: " << sum << std::endl;
    return 0;
}

bool valid_bag(const bag& max_bag, const bag& test_bag) {
    bool possible_flag = false;
    if (max_bag.red_cubes   >= test_bag.red_cubes   &&
        max_bag.green_cubes >= test_bag.green_cubes &&
        max_bag.blue_cubes  >= test_bag.blue_cubes) {
       possible_flag = true;
    }
    return possible_flag;
}

void parse_bag(const std::string& round, bag& source) {
    static const std::regex RED_REGEX("\\d+\\s(?=red)");
    static const std::regex GREEN_REGEX("\\d+\\s(?=green)");
    static const std::regex BLUE_REGEX("\\d+\\s(?=blue)");

    int red_count = 0;
    int green_count = 0;
    int blue_count = 0;

    std::smatch match;
    if (std::regex_search(round, match, RED_REGEX)) {
        red_count = std::stoi(match.str());
    }
    if (std::regex_search(round, match, GREEN_REGEX)) {
        green_count = std::stoi(match.str());
    }
    if (std::regex_search(round, match, BLUE_REGEX)) {
        blue_count = std::stoi(match.str());
    }

    source.update(red_count, green_count, blue_count);
}
