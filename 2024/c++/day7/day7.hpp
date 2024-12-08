#pragma once

#include <print>
#include <span>

#include "../util.h"

namespace day7_impl {
	int64_t concatNums(int64_t x, int64_t y) {
		auto strConcat = std::to_string(x) + std::to_string(y);
		int64_t ret = 0;
		std::from_chars(strConcat.data(), strConcat.data() + strConcat.size(), ret);
		return ret;
	}

	bool isSolveableImpl(const int64_t goal, int64_t total, const std::span<int64_t>& nums, size_t numIdx) {
		if (numIdx == nums.size()) {
			return total == goal;
		} else {
			return isSolveableImpl(goal, total + nums[numIdx], nums, numIdx + 1) ||
				   isSolveableImpl(goal, total * nums[numIdx], nums, numIdx + 1) ||
				   isSolveableImpl(goal, concatNums(total, nums[numIdx]), nums, numIdx + 1);
		}
	}

	bool isSolveable(int64_t goal, const std::span<int64_t>& nums) {
		assert(!nums.empty());
		return isSolveableImpl(goal, nums[0], nums, 1);
	}
}

inline void execDay7() {
	int64_t total = 0;

	forEachLine("src/day7/day7_input.txt", [&](std::string_view path) {
		auto colonPos = path.find(':');
		int64_t goal = 0;
		std::from_chars(path.data(), path.data() + colonPos, goal);
		
		auto remainingLen = (path.size() - colonPos);
		
		auto nums = vecSplit<int64_t>(path.substr(colonPos + 2, remainingLen), ' ');
		
		if (day7_impl::isSolveable(goal, nums)) {
			total += goal;
		}
	});

	std::println("{}", total);
}