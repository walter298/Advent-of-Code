#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Board {
public:
	int layout[5][5];
};

class Game {
public:
	std::vector<int> undrawnNums;
	std::vector<int> drawnNums;
	std::vector<Board> boards;
};

Board winningBoard;

bool winBoard(Game game, int index)
{
	int consecutiveMarkCount = 0;

	for (int i = 0; i <= 4; i++) {
		consecutiveMarkCount = 0;

		for (int j = 0; j <= 4; j++) {
			int last = consecutiveMarkCount;

			for (int& q : game.drawnNums) {
				if (q == game.boards[index].layout[i][j]) {
					consecutiveMarkCount++;

					if (consecutiveMarkCount == 5) {
						return true;
					}
					break;
				}
			}
			if (last == consecutiveMarkCount) {
				break;
			}
		}
	}

	consecutiveMarkCount = 0;

	for (Board& board : game.boards) {
		for (int i = 0; i <= 4; i++) {
			consecutiveMarkCount = 0;

			for (int j = 0; j <= 4; j++) {
				int last = consecutiveMarkCount;

				for (int& q : game.drawnNums) {
					if (q == board.layout[j][i]) {
						consecutiveMarkCount++;

						if (consecutiveMarkCount == 5) {
							winningBoard = board;
							return true;
						}
						break;
					}
				}
				if (last == consecutiveMarkCount) {
					break;
				}
			}
		}
	}

	return false;
}

bool won(Game game)
{
	int consecutiveMarkCount = 0;

	for (Board& board : game.boards) {
		for (int i = 0; i <= 4; i++) {
			consecutiveMarkCount = 0;

			for (int j = 0; j <= 4; j++) {
				int last = consecutiveMarkCount;

				for (int& q : game.drawnNums) {
					if (q == board.layout[i][j]) {
						consecutiveMarkCount++;

						if (consecutiveMarkCount == 5) {
							winningBoard = board;
							return true;
						}
						break;
					}
				}
				if (last == consecutiveMarkCount) {
					break;
				}
			}
		}
	}

	consecutiveMarkCount = 0;

	for (Board& board : game.boards) {
		for (int i = 0; i <= 4; i++) {
			consecutiveMarkCount = 0;

			for (int j = 0; j <= 4; j++) {
				int last = consecutiveMarkCount;

				for (int& q : game.drawnNums) {
					if (q == board.layout[j][i]) {
						consecutiveMarkCount++;

						if (consecutiveMarkCount == 5) {
							winningBoard = board;
							return true;
						}
						break;
					}
				}
				if (last == consecutiveMarkCount) {
					break;
				}
			}
		}
	}

	return false;
}

void getUndrawnNums(std::vector<int>& vector, std::string line)
{
	std::string currentNum;

	for (size_t i = 0; i < line.size(); i++) {
		if (isdigit(line[i])) {
			currentNum.push_back(line[i]);
		} else {
			vector.push_back(std::stoi(currentNum));
			currentNum.clear();
		}
	}
}

void fillRow(Board& board, int row, std::string line) {
	std::string currentNum;

	int j = 0;

	for (size_t i = 0; i <= line.size(); i++) {
		if (i == line.size()) {
			board.layout[row][j] = std::stoi(currentNum);

			break;
		}
		if (line[i] != ' ') {
			currentNum.push_back(line[i]);
		} else {
			if (!currentNum.empty()) {
				board.layout[row][j] = std::stoi(currentNum);

				j++;
				
				currentNum.clear();
			}
		}
	}
}

int sumOfBoard(Board board, std::vector<int> markedNumbers)
{
	int sum = 0;

	bool currentlyMarked = false;

	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			for (int& n : markedNumbers) {
				if (board.layout[i][j] == n) {
					currentlyMarked = true;
					break;
				}
			}
			if (!currentlyMarked) {
				sum += winningBoard.layout[i][j];
			} else {
				currentlyMarked = false;
			}
		}
	}

	return sum;
}

Game gameInput()
{
	Game game;

	std::vector<Board> boards;

	std::ifstream inputFile;
	inputFile.open("input.txt");

	int i = 0;
	
	std::string line;

	Board board;
	int currentRow = 0;

	while (std::getline(inputFile, line)) {
		if (i == 0) {
			getUndrawnNums(game.undrawnNums, line);

			i = 1;

			continue;
		}

		if (!line.empty()) {
			fillRow(board, currentRow, line);

			currentRow++;
		}

		if (currentRow == 5) {
			game.boards.push_back(board);
			
			currentRow = 0;
		}
	}

	return game;
}

int main()
{
	Game game = gameInput();
	Game game2 = gameInput();

	while (true) {
		game.drawnNums.push_back(game.undrawnNums[0]);
		game.undrawnNums.erase(game.undrawnNums.begin() + 0);

		if (won(game)) {
			std::cout << "we have a winner!\n";
			
			for (int& i : game.drawnNums) {
				std::cout << i << std::endl;
			}

			for (int i = 0; i <= 4; i++) {
				std::cout << std::endl;
				for (int j = 0; j <= 4; j++) {
					std::cout << winningBoard.layout[i][j] << " ";
				}
			}

			std::cout << "the first answer is " << sumOfBoard(winningBoard, game.drawnNums) * game.drawnNums[game.drawnNums.size() - 1];

			break;
		}
	}
}