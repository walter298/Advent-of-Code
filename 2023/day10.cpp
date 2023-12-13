#include "day10.h"

class Graph {
private:
	static constexpr int DIM = 140;

	struct Node {
		bool north = false;
		bool west = false;
		bool south = false;
		bool east = false;

		bool visited = false;

		Node(char c);
		Node() = default;
	};

	int m_startX = 0;
	int m_startY = 0;

	using Grid = Node[DIM][DIM];
	Grid m_grid;

	static std::string coordStr(int x, int y);

	bool connects(int x1, int y1, int x2, int y2) const;

	using Coord = std::pair<int, int>;
	std::vector<Coord> nodeDestinations(int x, int y) const;
public:
	Graph();
	int longestDistanceFromStart();
};

std::string Graph::coordStr(int x, int y) {
	return '[' + std::to_string(x) + ", "s + std::to_string(y) + ']';
}

Graph::Node::Node(char c) {
	switch (c) {
	case '|':
		north = true;
		south = true;
		break;
	case '-':
		west = true;
		east = true;
		break;
	case 'L':
		north = true;
		east = true;
		break;
	case 'J':
		west = true;
		north = true;
		break;
	case '7':
		west = true;
		south = true;
		break;
	case 'F':
		east = true;
		south = true;
		break;
	}
}

bool Graph::connects(int x1, int y1, int x2, int y2) const {
	if (y2 < 0 || y2 >= DIM || x2 < 0 || x2 >= DIM) {
		return false;
	}

	const Node& start = m_grid[x1][y1];
	const Node& dest = m_grid[x2][y2];

	if (y2 < y1) {
		return start.north && dest.south; 
	} else if (y2 > y1) {
		return start.south && dest.north;
	} else if (x2 > x1) {
		return start.east && dest.west;
	} else if (x2 < x1) {
		return start.west && dest.east;
	}
	return false;
}

std::vector<Graph::Coord> Graph::nodeDestinations(int x, int y) const {
	std::vector<Coord> ret;
	std::array possibleNodes{
		std::pair{ x, y + 1 }, //north port
		std::pair{ x, y - 1 }, //south port
		std::pair{ x - 1, y }, //west port
		std::pair{ x + 1, y } //east port
	};
	for (const auto& [x2, y2] : possibleNodes) {
		if (connects(x, y, x2, y2)) {
			ret.emplace_back(x2, y2);
		}
	}
	return ret;
}

Graph::Graph() {
	int x = 0;
	int y = 0;
	bool foundStartPos = false;

	advent::doWhileReading(
		"2023/inputs/day10.txt"s,
		[&, this](const auto& line) {
			if (!foundStartPos) {
				auto startPos = std::ranges::find(line, 'S');
				if (startPos != line.end()) {
					m_startX = static_cast<int>(std::distance(line.begin(), startPos));
					m_startY = y;
					foundStartPos = true;
				}
			}
			for (const auto& chr : line) {
				m_grid[x][y] = chr;
				x++;
			}
			x = 0;
			y++;
		}
	);

	//the start position is willing to connect to anything
	auto& start = m_grid[m_startX][m_startY];
	start.north = true;
	start.west = true;
	start.south = true;
	start.east = true;
}

int Graph::longestDistanceFromStart() {
	using Coord = std::pair<int, int>;

	//int represents how many nodes have been visited so far, pair.first = node that came before, pair.second = current node
	using PathPoint = std::pair<int, std::pair<Coord, Coord>>;

	std::queue<PathPoint> paths;
	paths.emplace(-1, std::pair{ Coord{ m_startX, m_startY }, Coord{ m_startX, m_startY } });

	m_grid[m_startX][m_startY].visited = true;
	
	int greatestVisitC = 0;

	while (!paths.empty()) {
		auto [nodesVisited, nodes] = paths.front(); //INTENTIONAL COPY
		const auto& [lastNodeCoord, currNodeCoord] = nodes;

		//visit current node
		const auto& [cx, cy] = currNodeCoord;
		m_grid[cx][cy].visited = true;

		//std::cout << "Visiting " << coordStr(cx, cy) << " from " << coordStr(lastNodeCoord.first, lastNodeCoord.second) << '\n';
		nodesVisited++; 

		paths.pop();

		auto dests = nodeDestinations(cx, cy);
		for (const auto& dest : dests) {
			const auto& [destX, destY] = dest;
			if (m_grid[destX][destY].visited && dest != lastNodeCoord) { //if we've reached cycle
				if (nodesVisited > greatestVisitC) {
					greatestVisitC = nodesVisited;
				}
			} else if (!m_grid[destX][destY].visited) {
				paths.emplace(nodesVisited, std::pair{ currNodeCoord, dest });
			}
		}
	}

	return greatestVisitC;
}

void runDay10() {
	Graph g;
	std::cout << g.longestDistanceFromStart() << '\n';
}