#pragma once

#include <assert.h>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>

using namespace std::literals;

namespace advent {
	template<bool delta, std::weakly_incrementable It, std::invocable<typename It::value_type> Pred>
	void advanceUntil(It& it, It end, Pred&& pred) {
		while (it != end && !pred(*it)) {
			if constexpr (delta) {
				it++;
			} else  {
				it--;
			} 
		}
	}

	template<bool move, std::weakly_incrementable It, typename Callback>
	void getWords(It begin, It end, Callback&& c) {
		if (begin == end) { return; }

		do {
			advanceUntil<true>(begin, end, [](const char& c) { return !std::isspace(c); });
			if (begin == end) { return; }
			auto lastIt = std::next(begin);
			advanceUntil<true>(lastIt, end, [](const char& c) { return std::isspace(c); });

			std::string substr;
			if constexpr (move) {
				substr = {
					std::make_move_iterator(begin),
					std::make_move_iterator(lastIt)
				};
			}
			else {
				substr = { begin, lastIt };
			}
			c(substr);
			begin = lastIt;
		} while (begin != end);
	}

	template<bool move, std::ranges::random_access_range String, typename Callback>
	void getWords(String&& s, Callback&& c) {
		getWords<move>(s.begin(), s.end(), std::forward<Callback>(c));
	}

	template<typename T>
	consteval bool isNothrowForwardContructible() {
		if constexpr (std::is_lvalue_reference_v<T>) {
			return std::is_nothrow_copy_constructible_v<T>;
		} else {
			return std::is_nothrow_move_constructible_v<T>;
		}
	}
	template<typename T>
	consteval bool isNothrowForwardAssignable() {
		if constexpr (std::is_lvalue_reference_v<T>) {
			return std::is_nothrow_copy_assignable_v<T>;
		} else {
			return std::is_nothrow_move_assignable_v<T>;
		}
	}

	template<typename String, typename Callback>
	void doWhileReading(const String& relativePath, Callback c) {
		const String workingDirectory = "D:/C++_Projects_V2/Advent_of_Code/Redux/Redux";

		//open the file
		std::ifstream file{ workingDirectory + "/" + relativePath };
		assert(file.is_open());

		std::string line;
		while (std::getline(file, line)) {
			c(line);
		}
		file.close();
	}

	template<std::convertible_to<const char*> String, typename Callback, typename... Args>
	void scanForEachLine(const String& relativePath, Callback c, Args... args) {
		//open the file
		const String workingDirectory = "D:/C++_Projects_V2/Advent_of_Code/Redux/Redux";
		std::fstream file{ workingDirectory + "/" + relativePath };
		assert(file.is_open());

		std::istringstream iss;
		std::string line;
		while (std::getline(file, line)) {
			iss.clear();
			iss.str(line);
			((iss >> args), ...);
			c(args...);
		}
		file.close();
	}

	template<std::ranges::viewable_range Range>
	void print(const Range& r, std::string name = "") {
		if (!name.empty()) {
			std::cout << name << " ";
		}
		for (const auto& elem : r) {
			std::cout << elem << " ";
		}
		std::cout << '\n';
	}
}