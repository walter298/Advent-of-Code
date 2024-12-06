#pragma once

#include <charconv>
#include <ranges>
#include <string_view>
#include <tuple>
#include <vector>

namespace detail {
	void convertFromVal(auto& num, auto& numStrVal) {
		const auto numStrPtrBegin = &(*numStrVal.begin());
		const auto numStrPtrEnd   = &(*numStrVal.end()); //will raise runtime debug assertion (TURN OFF STL ITERATOR CHECKS)

		std::from_chars(numStrPtrBegin, numStrPtrEnd, num);
	}

	void convert(auto& num, auto& numStrIt) {
		auto numStr = *numStrIt;
		convertFromVal(num, numStr);
		numStrIt++;
	}

	using SplitRangeType = decltype(std::declval<std::string_view>() | std::views::split(','));

	template<typename Tuple, size_t... Idxs>
	void assignSplitNumsImpl(Tuple& tuple, SplitRangeType splitRange, std::index_sequence<Idxs...>) {
		auto currIt = splitRange.begin();
		((convert(std::get<Idxs>(tuple), currIt)), ...);
	}
}

template<typename... Nums>
std::tuple<Nums...> split(std::string_view str, char delim = ',') {
	std::tuple<Nums...> ret;
	detail::assignSplitNumsImpl(ret, str | std::views::split(delim), std::make_index_sequence<sizeof...(Nums)>{});
	return ret;
}

template<typename Num>
std::vector<Num> vecSplit(std::string_view str, char delim = ',') {
	auto splitView = str | std::views::split(delim);
	std::vector<Num> nums(static_cast<size_t>(std::distance(splitView.begin(), splitView.end())));
	
	for (auto&& [num, numStr] : std::views::zip(nums, splitView)) {
		detail::convertFromVal(num, numStr);
	}

	return nums;
}