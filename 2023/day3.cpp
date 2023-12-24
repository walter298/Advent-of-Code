#include "day3.h"

constexpr int ROW_LEN = 10;
constexpr int LAST_ROW_IDX = 9;

using RowWindow = std::array<std::string, 3>;

using StringIt = std::string::const_iterator;
bool containsSpecialChar(StringIt x1, StringIt x2, StringIt end) {
	return (std::ranges::find_if(x1, x2, [](const auto& chr) { return chr != '.'; }) != end);
}

int sumPartNums(const std::string& currRow, const auto&... surroundingRows) {
	std::cout << "Current Row: " << currRow << '\n';
	std::cout << "Surrounding Row(s): ";
	((std::cout << surroundingRows << " "), ...);
	std::cout << "\n\n";

	int ret = 0;

	auto currPos = currRow.cbegin();

	while(currPos != currRow.cend()) {
		currPos = std::ranges::find_if(currPos, currRow.cend(), isdigit);
		if (currPos == currRow.cend()) { //if there are no more numbers in the row, break
			break;
		}
		const auto numX1 = static_cast<size_t>(std::distance(currRow.cbegin(), currPos)); //position of first char
		//increment currPos until we reach end of row or non-digit char
		advent::advanceUntil<true>(currPos, currRow.cend(),
			[](const auto& it) { return !(isdigit(it)); }
		);
		const auto numX2 = static_cast<size_t>(std::distance(currRow.cbegin(), currPos)); //position of last char

		//horizontal range to check for special chars
		auto rangeX1 = numX1;
		auto rangeX2 = numX2;

		//expand x1-x2 to diagonals if possible
		if (rangeX1 > 0) {
			rangeX1--;
			if (currRow[rangeX1] != '.') {
				ret += std::stoi(currRow.substr(numX1, numX2 - numX1));
				continue;
			}
		}
		if (rangeX2 < currRow.size()) {
			rangeX2++;
			if (currRow[rangeX2] != '.') {
				ret += std::stoi(currRow.substr(numX1, numX2 - numX1));
				continue;
			}
		}

		//check surrounding rows for special chars
		if ((... || containsSpecialChar(surroundingRows.begin() + rangeX1, surroundingRows.begin() + rangeX2, 
										surroundingRows.end()))) 
		{
			ret += std::stoi(currRow.substr(numX1, numX2 - numX1));
		}
	};

	return ret;
}

void runDay3() {
	RowWindow currRows;

	size_t topIdx = 0;
	size_t middleIdx = 1;
	size_t bottomIdx = 2;
	auto shiftIndices = [&] {
		topIdx = (topIdx + 1) % 3;
		middleIdx = (middleIdx + 1) % 3;
		bottomIdx = (bottomIdx + 1) % 3;
	};

	int rowC = 0;

	advent::printSum(
		"2023/inputs/day3_test.txt"s,
		[&](const auto& line) {
			currRows[bottomIdx] = line;

			int sum = 0;
			switch (rowC) {
			case 0:
				sum = sumPartNums(currRows[topIdx], currRows[middleIdx]);
				break;
			case LAST_ROW_IDX:
				sum = sumPartNums(currRows[bottomIdx], currRows[middleIdx]);
				break;
			default:
				sum = sumPartNums(currRows[middleIdx], currRows[topIdx], currRows[bottomIdx]);
				shiftIndices();
				break;
			}

			rowC++;

			return sum;
		}, 
		//initially add 1 row
		[&](std::ifstream& file) {
			std::string line;

			std::getline(file, line);
			currRows[topIdx] = line;

			std::getline(file, line);
			currRows[middleIdx] = line;
		}
	);
}