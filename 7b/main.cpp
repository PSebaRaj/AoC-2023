#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

constexpr char map_card_to_alphabet_value(const char value)
{
	switch (value)
	{
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return 'b' + (value - '2');
	case 'T':
		return 'j';
	case 'J':
		return 'k';
	case 'Q':
		return 'l';
	case 'K':
		return 'm';
	case 'A':
		return 'n';
	default:
		return '0';
	}
}

typedef enum class WIN_TYPE : uint8_t
{
	HIGH_CARD = 0,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_KIND,
	FULL_HOUSE,
	FOUR_OF_KIND,
	FIVE_OF_KIND,
} Win_type;

struct Hand
{
	std::string cards;
	uint64_t bid;
	WIN_TYPE type;
};

WIN_TYPE determine_win_type(std::string hand, std::string unique_chars)
{
	uint32_t max_appearances = 0;
	for (const auto &c : unique_chars)
	{
		auto appearances = static_cast<uint32_t>(std::count(hand.begin(), hand.end(), c));
		max_appearances = (appearances > max_appearances) ? appearances : max_appearances;
	}

	if (max_appearances == 4)
	{
		return WIN_TYPE::FOUR_OF_KIND;
	}
	if (max_appearances == 2)
	{
		return WIN_TYPE::TWO_PAIR;
	}
	if (max_appearances == 3 && unique_chars.length() == 3)
	{
		return WIN_TYPE::THREE_OF_KIND;
	}

	return WIN_TYPE::FULL_HOUSE;
}

std::vector<Hand> read_input_file(std::string input_file)
{
	std::vector<Hand> hands = {};
	std::ifstream file_input;

	file_input.open(input_file);

	if (file_input.is_open())
	{
		for (std::string line; std::getline(file_input, line);)
		{
			if (line.length() > 0)
			{
				std::stringstream ss(line);
				std::string hand = {};
				ss >> hand;
				uint32_t bid = {};
				ss >> bid;
				// convert hand to basic alphabet representation
				for (auto &card : hand)
				{
					card = map_card_to_alphabet_value(card);
				}

				std::string unique_chars = hand;
				std::sort(unique_chars.begin(), unique_chars.end());

				const auto index = std::unique(unique_chars.begin(), unique_chars.end());
				unique_chars = unique_chars.substr(0, index - unique_chars.begin());
				const uint64_t unique_char_length = unique_chars.length();

				Win_type win_type = {};

				switch (unique_char_length)
				{
				case 1:
					win_type = WIN_TYPE::FIVE_OF_KIND;
					break;
				case 4:
					win_type = WIN_TYPE::ONE_PAIR;
					break;
				case 5:
					win_type = WIN_TYPE::HIGH_CARD;
					break;
				default:
					win_type = determine_win_type(hand, unique_chars);
				}
				hands.push_back({hand, bid, win_type});
			}
		}
		file_input.close();
	}
	return hands;
}

void sort_hands(std::vector<Hand> &hands)
{
	// sort hands by order of strength by win type then by order of cards in hand
	std::sort(hands.begin(), hands.end(), [](const Hand &a, const Hand &b)
			  {
		   if (a.type == b.type) {
			   return a.cards < b.cards;
		   }
		   return a.type < b.type; });
}

uint64_t accumulate_winnings(const std::vector<Hand> &hands)
{
	uint64_t accumulated_winnings = {};
	for (int i = 0; i < hands.size(); ++i)
	{
		accumulated_winnings += (i + 1) * hands[i].bid;
	}
	return accumulated_winnings;
}

uint64_t new_total_winnings(std::vector<Hand> &hands)
{
	constexpr char wildcard = map_card_to_alphabet_value('J');
	for (auto &[cards, bid, type] : hands)
	{
		if (cards.find(wildcard) != std::string::npos)
		{
			// we already had the win type, just need to promote as best as possible
			if (type == WIN_TYPE::HIGH_CARD)
			{
				type = WIN_TYPE::ONE_PAIR; // high card promotes to one pair
			}
			else if (type == WIN_TYPE::ONE_PAIR)
			{
				type = WIN_TYPE::THREE_OF_KIND;
			}
			else if (type == WIN_TYPE::TWO_PAIR)
			{
				// depends on wildcard count on how promotion works 2 wild cards goes to 4 of a kind, 1 moves to full house
				const auto wildcard_count = static_cast<uint32_t>(std::count(cards.begin(), cards.end(), wildcard));
				type = (wildcard_count > 1) ? WIN_TYPE::FOUR_OF_KIND : WIN_TYPE::FULL_HOUSE;
			}
			else if (type == WIN_TYPE::THREE_OF_KIND)
			{
				type = WIN_TYPE::FOUR_OF_KIND;
			}
			else if (type == WIN_TYPE::FULL_HOUSE || type == WIN_TYPE::FOUR_OF_KIND)
			{
				type = WIN_TYPE::FIVE_OF_KIND; // if we have full house or four of a kind must promote to five of kind
			}

			std::replace(cards.begin(), cards.end(), wildcard, 'a');
		}
	}

	sort_hands(hands);
	return accumulate_winnings(hands);
}

int main()
{
	std::vector<Hand> hands = read_input_file("input.txt");

	std::cout << new_total_winnings(hands) << std::endl;
}
