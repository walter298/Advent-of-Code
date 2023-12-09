#include "day1.h"

int sum = 0;

void concatFirstAndLastDigits(const std::string& line) {
	auto first = *std::ranges::find_if(line, isdigit);
	auto last = *std::ranges::find_if(line | std::views::reverse, isdigit);
	sum += std::stoi(std::string{ first, last });
}

void runDay1Part1() {
	advent::doWhileReading("2023/inputs/day1.txt"s, concatFirstAndLastDigits);
	std::cout << sum << std::endl;
}

int sum2 = 0;

void concatFirstAndLastWordDigits(const std::string& line) {
	const std::array wordDigits = {
		"one"s,
		"two"s,
		"three"s,
		"four"s,
		"five"s,
		"six"s,
		"seven"s,
		"eight"s,
		"nine"s
	};
}

void runDay1Part2() {
	advent::doWhileReading("2023/inputs/day1_part2.txt"s, concatFirstAndLastWordDigits);
	std::cout << sum2 << std::endl;
}