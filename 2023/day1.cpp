#include "day1.h"

int concatFirstAndLastDigits(const std::string& line) {
	auto first = *std::ranges::find_if(line, isdigit);
	auto last = *std::ranges::find_if(line | std::views::reverse, isdigit);
	return std::stoi(std::string{ first, last });
}

void runDay1Part1() {
	advent::printSum("2023/inputs/day1.txt"s, concatFirstAndLastDigits);
}

int concatFirstAndLastWordDigits(const std::string& line) {
	using WordDigits = boost::container::flat_map<std::string, char>;
	const WordDigits wordDigits = {
		{ "one"s, '1' },
		{ "two"s, '2' },
		{ "three"s, '3' },
		{ "four"s, '4', },
		{ "five"s, '5', },
		{ "six"s, '6' },
		{ "seven"s, '7' },
		{ "eight"s, '8' },
		{ "nine"s, '9' }
	};
	auto searchForAny = [&](auto... views) {
		auto lineView = (line | ... | views);
		int64_t currShortestDist = std::numeric_limits<int64_t>::max();
		char ret = '0';

		// get position of char in string
		auto chrPos = std::ranges::find_if(lineView, isdigit);
		// if chr exists in string, get distance from chrPos to beginning of range
		if (chrPos != lineView.end()) {
			currShortestDist = std::ranges::distance(lineView.begin(), chrPos);
			ret = *chrPos;
			if (currShortestDist <= 3) {
				return ret;
			}
		}

		for (const auto& [word, chr] : wordDigits) {
			auto wordPos = std::ranges::search(lineView, (word | ... | views));
			if (!wordPos.empty()) {
				auto dist = std::ranges::distance(lineView.begin(), wordPos.begin());
				if (dist < currShortestDist) {
					currShortestDist = dist;
					ret = chr;
				}
			}
		}
		return ret;
	};
	const auto first = searchForAny();
	const auto second = searchForAny(std::views::reverse);
	return std::stoi(std::string{ first, second });
}

void runDay1Part2() {
	advent::printSum("2023/inputs/day1.txt"s, concatFirstAndLastWordDigits);
}