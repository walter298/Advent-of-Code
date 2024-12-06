#include <algorithm>
#include <cassert>
#include <fstream>
#include <print>
#include <vector>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "../util.h"

namespace {
	using Edges      = boost::unordered_flat_set<int>;
	using PrintOrder = boost::unordered_flat_map<int, Edges>;
	using Update     = std::vector<int>;
	using Page       = Update::iterator;

	struct ProblemData {
		PrintOrder order;
		std::vector<Update> updates;
	};

	void loadOrder(std::ifstream& file, std::string& currLine, PrintOrder& order) {
		constexpr int ORDER_COUNT = 1176;

		for (int i = 0; i < ORDER_COUNT; i++) {
			std::getline(file, currLine);
			auto [a, b] = split<int, int>(currLine, '|');
			order[a].insert(b);
		}
	}

	void loadUpdates(std::ifstream& file, std::string& currLine, std::vector<Update>& updates) {
		while (std::getline(file, currLine)) {
			updates.push_back(vecSplit<int>(currLine));
		}
	}

	ProblemData loadData() {
		ProblemData ret;

		std::ifstream file{ "src/day5/day5_input.txt" };
		assert(file.is_open());

		std::string currLine;

		loadOrder(file, currLine, ret.order);
		loadUpdates(file, currLine, ret.updates);

		return ret;
	}

	//update can be const or non-const
	auto misplacedEdgeIt(auto& update, const PrintOrder& printOrder) {
		auto findMisplacedEdge = [&](int n, auto begin, auto end) {
			return std::ranges::find_if(begin, end, [&](auto prevNum) {
				if (!printOrder.contains(prevNum)) { //we are good if no numbers need to go before *it
					return false;
				} else {
					return printOrder.at(prevNum).contains(n);
				}
			});
		};
		for (auto it = update.begin(); it != update.end(); it++) {
			auto misplacedEdgeIt = findMisplacedEdge(*it, std::next(it), update.end());
			if (misplacedEdgeIt != update.end()) {
				return misplacedEdgeIt;
			}
		}
		return update.end();
	}

	int part1(const ProblemData& data) {
		int total = 0;
		for (const auto& update : data.updates) {
			if (misplacedEdgeIt(update, data.order) == update.end()) { //if update is CORRECT
				total += update[update.size() / 2];
			}
		}
		return total;
	}

	Update::iterator findEarliestEdge(Update& update, const Edges& edges) {
		for (auto it = update.begin(); it != update.end(); it++) {
			if (edges.contains(*it)) {
				return it;
			}
		}
		return update.end();
	}

	void insert(int n, Update& update, const PrintOrder& order);
	
	void insertWithIteratorHint(Page pos, Update& update, const PrintOrder& order) {
		int n = *pos;

		//if number has no edges, we don't need to move it anywhere
		auto edgesIt = order.find(n);
		if (edgesIt == order.end()) {
			//std::println("{} has no edges", n);
			return;
		}

		//insert the number before the previous edge
		const auto& edges = edgesIt->second;
		assert(!edges.empty());
		auto earliestEdgeIt = findEarliestEdge(update, edges); 
		if (earliestEdgeIt == update.end()) { //if there are no edges, return
			return;
		}

		assert(pos != update.end());

		std::rotate(earliestEdgeIt, pos, std::next(pos));
	}

	int midpointOfFixedUpdate(Update& update, const PrintOrder& order) {
		bool isCorrect = true;

		while (true) {
			auto outOfPlaceNum = misplacedEdgeIt(update, order);
			if (outOfPlaceNum != update.end()) {
				isCorrect = false;
				insertWithIteratorHint(outOfPlaceNum, update, order);
			} else {
				break;
			}
		}

		return isCorrect ? 0 : update[update.size() / 2];
	}

	void insert(int n, Update& update, const PrintOrder& order) {
		//if number is already in the fixed update, we can exit
		auto nIt = std::ranges::find(update, n);
		if (nIt == update.end()) {
			return;
		}
		insertWithIteratorHint(nIt, update, order);
	}

	int part2(ProblemData& data) {
		int total = 0;
		for (auto& update : data.updates) {
			total += midpointOfFixedUpdate(update, data.order);
		}
		return total;
	}
}

inline void exec() {
	auto data = loadData();

	std::println("{}", part1(data));
	std::println("{}", part2(data));
}