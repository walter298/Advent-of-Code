#include "day9.h"

constexpr bool PART_2 = true;

constexpr size_t NUM_C = 21; //formally 21
using Differences = std::array<int, NUM_C>;

int nextInt(std::istringstream& iss) {
	Differences differences;
	auto currEnd = differences.end();

	for (auto& num : differences) {
		iss >> num;
	}

	//while not all of the elements in differences are the same
	while (true) {
		int first = differences.front(); 
		auto currDiffs = std::ranges::subrange(differences.begin(), currEnd);
		if (std::ranges::adjacent_find(currDiffs, std::not_equal_to<int>()) == currEnd) {
			break;
		}
		for (auto [a, b] : currDiffs | std::views::adjacent<2>) {
			a = b - a;
		}
		currEnd--;
		if constexpr (PART_2) {
			*currEnd = first;
		}
	}
	if constexpr (PART_2) {
		//advent::print(differences, "Full Differences:");
		int last = *std::prev(currEnd);
		int ret = last;
		for (const auto& diff : std::ranges::subrange(currEnd, differences.end())) {
			//std::cout << diff << " - " << last << ": " << diff - last << '\n';
			ret = diff - last;
			last = ret;
		}
		return ret;
	} else {
		return std::accumulate(std::prev(currEnd), differences.end(), 0);
	}
	//return std::accumulate(std::prev(currEnd), differences.end(), 0);
}

void runDay9() {
	int sum = 0;
	
	std::istringstream iss;
	advent::doWhileReading("2023/inputs/day9.txt"s,
		[&sum, &iss](const auto& line) {
			iss.str(line);
			sum += nextInt(iss);
			iss.clear();
		}
	);
	std::cout << sum << std::endl;
}