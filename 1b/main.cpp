#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

int find_first_word_digit(std::string currentLine, int digitPos, std::string wordDigits[]){
    int wordDigitPos;
    int index = -1;

    for(int i = 0; i < 9; i++){
        wordDigitPos = currentLine.find(wordDigits[i]);
        if(wordDigitPos < digitPos && wordDigitPos != std::string::npos){
            digitPos = wordDigitPos;
            index = i;
        }
    }

    return index + 1;
}

int find_last_word_digit(std::string currentLine, int digitPos, std::string wordDigits[]){
    int wordDigitPos;
    int index = -1;

    for(int i = 0; i < 9; i++){
        wordDigitPos = currentLine.rfind(wordDigits[i]);
        if(wordDigitPos > digitPos && wordDigitPos != std::string::npos){
            digitPos = wordDigitPos;
            index = i;
        }
    }

    return index + 1;
}

int find_calibration_value(std::string line) {
	std::string currentNum = "";
	int digitPos, wordDigitNum;
	std::string wordDigits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

	digitPos = line.find_first_of("123456789");
	wordDigitNum = find_first_word_digit(line, digitPos, wordDigits);
	if(wordDigitNum > 0){
		currentNum += std::to_string(wordDigitNum);
	}else{
		currentNum += line[digitPos];
	}

	digitPos = line.find_last_of("0123456789");
	wordDigitNum = find_last_word_digit(line, digitPos, wordDigits);
	if(wordDigitNum > 0){
		currentNum += std::to_string(wordDigitNum);
	}else{
		currentNum += line[digitPos];
	}

	return std::stoi(currentNum);
}

int main() {
	std::ifstream file("input.txt");
	std::int32_t sum = 0;
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			sum += find_calibration_value(line);
		}
		file.close();
	}
	printf("Result: %d\n", sum);
}
