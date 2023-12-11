#include "day8.h"

using Children = std::pair<std::string, std::string>;

struct Network {
	std::string directions;
	std::vector<std::string> beginningNodes;
	std::unordered_map<std::string, Children> map;
};

int stepCPart2(Network& network) {
	int ret = 0;
	
	auto goInDirection = [&network](auto memp) {
		for (auto& node : network.beginningNodes) {
			node = network.map.at(node).*memp;
		}
	};

	while (true) {
		for (const auto& dir : network.directions) {
			auto memp = (dir == 'L') ? &Children::first : &Children::second;
			goInDirection(memp);
			ret++;
		}
		if (std::ranges::all_of(network.beginningNodes, [](const auto& n) { return n.back() == 'Z'; })) {
			return ret;
		}
	}

	std::unreachable();
}

int stepC(const Network& network) {
	int ret = 0;
	std::string currNode = "AAA";
	while (true) {
		for (const auto& dir : network.directions) {
			if (dir == 'L') {
				currNode = network.map.at(currNode).first;
			} else {
				currNode = network.map.at(currNode).second;
			}
			ret++;
			if (currNode == "ZZZ") {
				return ret;
			}
		}
	}
	std::unreachable();
}

Network makeNetwork() {
	Network ret;

	std::ifstream file{ advent::workingDirectory + "/2023/inputs/day8.txt" };
	assert(file.is_open());

	//get directions
	std::string line;
	std::getline(file, line);
	ret.directions = line;

	std::getline(file, line); //skip empty line

	while (std::getline(file, line)) {
		std::string node{ line.begin(), line.begin() + 3 };
		auto openParen = std::ranges::find(std::ranges::subrange(line.begin() + 3, line.end()), '(');
		std::string left{ openParen + 1, openParen + 4 };
		std::string right{ openParen + 6, openParen + 9 };
		ret.map[node] = { left, right };
		if (node.back() == 'A') {
			ret.beginningNodes.push_back(std::move(node));
		}
	}

	return ret;
}

void runDay8() {
	auto network = makeNetwork();
	std::cout << stepCPart2(network) << '\n';
}