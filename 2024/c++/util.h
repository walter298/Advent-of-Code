#pragma once

#include <cassert>
#include <charconv>
#include <fstream>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include <boost/container/static_vector.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/pfr.hpp>

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

std::vector<std::string> readFileLines(std::string_view path) {
	std::ifstream file{ path.data() };
	assert(file.is_open());

	std::vector<std::string> lines;
	std::string line;

	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	return lines;
}

template<std::invocable<std::string_view> Func>
void forEachLine(std::string_view path, Func f) {
	std::ifstream file{ path.data() };
	assert(file.is_open());

	std::string line;
	while (std::getline(file, line)) {
		f(line);
	}
}

template<typename T, size_t Size>
class StaticCircularBuffer {
private:
	using Data = boost::container::static_vector<T, Size>;
	Data m_data;
	size_t m_idx = 0;
public:
	constexpr StaticCircularBuffer() 
		: m_data(Size) 
	{
	}

	size_t size() const {
		return m_data.size();
	}
	
	const Data& data() const {
		return m_data;
	}

	template<typename U>
	const T& insert(U&& value) {
		m_data[m_idx] = std::forward<U>(value);
		m_idx = m_idx == Size ? 0 : m_idx + 1;
	}
};

namespace boost {
	template<typename T> requires(std::is_aggregate_v<T>)
	struct hash<T> {
		size_t operator()(const T& aggr) const noexcept {
			size_t hashCode = 0;
			boost::pfr::for_each_field(aggr, [&](const auto& member) {
				boost::hash_combine(hashCode, member);
			});
			return hashCode;
		}
	};
}