#include "day2.h"

int getPoints(const std::string& line) {
	std::istringstream iss{ line };
	
	//skip over "game: "
	constexpr size_t GAME_STR_C = 5;
	iss.ignore(GAME_STR_C);

	//get game ID
	int gameID = 0;
	iss >> gameID;
	
	//skip over ": "
	constexpr size_t COLOR_INTERSTICE_C = 2;
	iss.ignore(COLOR_INTERSTICE_C);

	constexpr int MAX_RED_C = 12;
	constexpr int MAX_GREEN_C = 13;
	constexpr int MAX_BLUE_C = 14;

	int currRedC = 0;
	int currGreenC = 0;
	int currBlueC = 0;

	bool isPossible = true;
	bool doneDrawing = false;

	while (!iss.eof()) {
		int colorC;
		std::string color;
		iss >> colorC >> color;
		if (color.back() == ';' || isalpha(color.back())) {
			doneDrawing = true;
		} 
		if (!isalpha(color.back())) {
			color.pop_back();
		}
		int maxColorC = 0;
		if (color == "red") {
			maxColorC = MAX_RED_C;
		} else if (color == "green") {
			maxColorC = MAX_GREEN_C;
		} else if (color == "blue") {
			maxColorC = MAX_BLUE_C;
		}
		if (colorC > maxColorC) {
			isPossible = false;
			break;
		}
	}
	//std::cout << "Returning: " << (isPossible ? gameID : 0) << '\n';
	return isPossible ? gameID : 0;
}

void runDay2() {
	advent::printSum("2023/inputs/day2.txt", getPoints);
}

int getPointsPart2(const std::string& line) {
	std::istringstream iss{ line };
	iss.ignore(100, ':');
	iss.ignore(1);

	int minRedC = 0;
	int minGreenC = 0;
	int minBlueC = 0;

	bool isPossible = true;
	bool doneDrawing = false;

	while (!iss.eof()) {
		int colorC;
		std::string color;
		iss >> colorC >> color;
		if (!isalpha(color.back())) {
			color.pop_back();
		}
		if (color == "red") {
			minRedC = std::max(minRedC, colorC);
		} else if (color == "green") {
			minGreenC = std::max(minGreenC, colorC);
		} else if (color == "blue") {
			minBlueC = std::max(minBlueC, colorC);
		}
	}
	return minRedC * minGreenC * minBlueC;
}

void runDay2Part2() {
	advent::printSum("2023/inputs/day2.txt", getPointsPart2);
}