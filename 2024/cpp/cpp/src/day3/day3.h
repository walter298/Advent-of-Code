#include <cassert>
#include <fstream>
#include <print>
#include <ranges>
#include <string_view>
#include <iostream>

#include <re2/re2.h>

namespace {
	std::string getFileInput() {
		std::ifstream file{ "src/day3/day3_input.txt" };
		assert(file.is_open());

		std::string ret;
		std::string line;
		while (std::getline(file, line)) {
			ret.append(line);
		}
		return line;
	}

	std::tuple<int, int> parseMul(re2::StringPiece& str) {
		std::array nums{ 0, 0 };
		size_t numIdx = 0;

		for (const auto& numStr : str | std::views::split(',')) {
			std::from_chars(numStr.begin(), numStr.end(), nums[numIdx]);
			numIdx++;
		}

		return { nums[0], nums[1] };
	}

	struct Mul {
		int x;
		int y;
		size_t idx;
	};

	
}

namespace day3 {
	inline void exec() {
		auto fileInput = getFileInput();
		
		// The pattern (substring) you're looking for
		constexpr auto MUL_PATTERN = R"(mul\(([0-9,]+)\)|(do\(\)))";
		// Create a RE2 object for matching the pattern
		re2::RE2 re(MUL_PATTERN);

		// This will hold all matches
		std::vector<std::string> matches;

		// Start searching for matches
		absl::string_view remaining = fileInput;
		while (RE2::FindAndConsume(&remaining, re, &matches.emplace_back())) {
			// Do something with each match, here it's simply printing it
			std::cout << "Found match: " << matches.back() << std::endl;
		}

		////constexpr auto MUL_PATTERN = R"(mul\(([0-9,]+)\)|(do\(\)))";
		//constexpr auto MUL_PATTERN = R"(d+)";
		//re2::StringPiece input{ fileInput };
		//re2::StringPiece match;

		//std::vector<Mul> muls;

		//while (RE2::FindAndConsume(&input, MUL_PATTERN, &match)) {
		//	std::println("{}", std::string{ match });
		//	/*auto [a, b] = parseMul(match);
		//	auto matchIdx = static_cast<size_t>(std::distance(input.begin(), match.begin()));
		//	muls.emplace_back(a, b, matchIdx);*/
		//}
	}
}