#pragma once

#include <optional>

#include <boost/multi_array.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "../util.h"

namespace day6_impl {
	using Map = boost::multi_array<char, 2>;
	using MapRange = boost::multi_array_types::index_range;

	struct Coord {
		int x = 0;
		int y = 0;

		bool operator==(const Coord& other) const {
			return x == other.x && y == other.y;
		}
	};

	struct Guard {
		int x = 0;
		int y = 0;
		int dx = 0;
		int dy = 0;

		void move() {
			x += dx;
			y += dy;
		}

		void rotate90Degrees() {
			if (dx != 0) {
				dy = -dx;
				dx = 0;
			}
			else if (dy != 0) {
				dx = dy;
				dy = 0;
			}
		}

		enum Dir {
			Up, Down, Left, Right
		};

		Dir orientation() const {
			if (dx != 0) {
				return dx == -1 ? Up : Down;
			}
			else {
				return dy == -1 ? Left : Right;
			}
		}

		Coord pos() const {
			return { x, y };
		}
	};

	struct ProblemData {
		Map map;
		Guard guard;
	};

	std::optional<Guard> findGuard(std::string_view line, size_t lineIdx) {
		auto guardCharIt = std::ranges::find_if(line, [](char c) {
			return c != '.' && c != '#';
			});
		if (guardCharIt == line.end()) {
			return std::nullopt;
		}
		auto guardChar = *guardCharIt;
		auto guardCharIdx = static_cast<size_t>(std::distance(line.begin(), guardCharIt));
		switch (guardChar) {
		case '^':
			return Guard{ static_cast<int>(lineIdx), static_cast<int>(guardCharIdx), -1, 0 };
			break;
		case '>':
			return Guard{ static_cast<int>(lineIdx), static_cast<int>(guardCharIdx), 0, -1 };
			break;
		case '<':
			return Guard{ static_cast<int>(lineIdx), static_cast<int>(guardCharIdx),  0, 1 };
			break;
		default:
			return Guard{ static_cast<int>(lineIdx), static_cast<int>(guardCharIdx), 1, 0 };
			break;
		}
	}

	ProblemData loadProblemData() {
		auto lines = readFileLines("src/day6/day6_input.txt");

		const auto rowC = lines.size();
		const auto colC = lines[0].size();
		Map map{ boost::extents[rowC][colC] };

		Guard guard;
		bool initializedGuard = false;

		for (const auto& [i, line] : std::views::enumerate(lines)) {
			if (!initializedGuard) {
				auto foundGuard = findGuard(line, static_cast<size_t>(i));
				if (foundGuard) {
					guard = *foundGuard;
					initializedGuard = true;
				}
			}
			std::ranges::move(line, map[i].begin());
		}
		return ProblemData{ std::move(map), guard };
	}

	bool inBounds(const Map& map, int x, int y) {
		return x >= 0 && std::cmp_less(x, map.size()) && y >= 0 && std::cmp_less(y, map[0].size());
	}

	enum class MoveResult {
		Rotated,
		MovedStraight,
		LeftMap
	};

	MoveResult move(const Map& map, Guard& guard) {
		auto x = guard.x + guard.dx;
		auto y = guard.y + guard.dy;
		if (!inBounds(map, x, y)) {
			return MoveResult::LeftMap;
		}

		if (map[x][y] == '#') {
			guard.rotate90Degrees();
			return MoveResult::Rotated;
		} else {
			guard.move();
			return MoveResult::MovedStraight;
		}
	}

	struct Move {
		Coord to;
		Guard::Dir dir{};

		bool operator==(const Move& other) const {
			return to == other.to && dir == other.dir;
		}
	};

	bool frontObstructionFormLoop(Map& map, Guard guard) {
		boost::unordered_flat_set<Move> visitedCoords;
		visitedCoords.emplace(guard.pos(), guard.orientation());

		auto obsX = guard.x + guard.dx;
		auto obsY = guard.y + guard.dy;
		if (!inBounds(map, obsX, obsY)) { //if guard is on the edge, return false
			return false;
		}

		if (map[obsX][obsY] == '#' || map[obsX][obsY] == 'X') {
			return false;
		}

		map[obsX][obsY] = '#';

		guard.rotate90Degrees(); //begin march
		
		while (move(map, guard) != MoveResult::LeftMap) {
			if (visitedCoords.contains({ guard.pos(), guard.orientation() })) {
				map[obsX][obsY] = '.';
				return true;
			}
			visitedCoords.emplace(guard.pos(), guard.orientation());
		}
		map[obsX][obsY] = '.';
		return false;
	}
}

inline void execDay6() {
	auto [map, guard] = day6_impl::loadProblemData();

	auto mapCopy = map;

	int totalUniqueSquaresVisited = 1;
	map[guard.x][guard.y] = 'X';

	int totalPossibleObstructions = 0;

	using day6_impl::MoveResult;
	while (true) {
		if (frontObstructionFormLoop(map, guard)) {
			totalPossibleObstructions++;
			auto obsX = guard.x + guard.dx;
			auto obsY = guard.y + guard.dy;
			mapCopy[obsX][obsY] = '0';
		}
		auto moveRes = day6_impl::move(map, guard);
		if (moveRes == MoveResult::MovedStraight && map[guard.x][guard.y] != 'X') {
			map[guard.x][guard.y] = 'X';
			totalUniqueSquaresVisited++;
		} else if (moveRes == MoveResult::LeftMap) {
			break;
		}
	}

	std::println("Part 1: {}", totalUniqueSquaresVisited);
	std::println("Part 2: {}", totalPossibleObstructions);

	for (int i = 0; i < mapCopy.size(); i++) {
		for (int j = 0; j < mapCopy[i].size(); j++) {
			std::print("{} ", mapCopy[i][j]);
		}
		std::println("");
	}
}