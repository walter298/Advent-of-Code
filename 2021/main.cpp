#include <iostream>
#include <vector>
#include <fstream>
#include <string>

int getIncreaseCount(std::vector<int> input)
{
	int deltaIncrease = 0;

	for (size_t i = 1; i < input.size(); i++) {
		if (input[i] > input[i - 1]) {
			deltaIncrease++;
		}
	}

	return deltaIncrease;
}

int getWindowIncreaseCount(std::vector<int> input) 
{
	int deltaIncrease = 0;

	int currentMeasurementTotal, lastMeasurementTotal = input[0] + input[1] + input[2];

	for (size_t i = 1; i < input.size(); i++) {
		if (i + 2 >= input.size()) {
			break;
		}

		currentMeasurementTotal = input[i] + input[i + 1] + input[i + 2];

		if (currentMeasurementTotal > lastMeasurementTotal) {
			deltaIncrease++;
		}

		lastMeasurementTotal = currentMeasurementTotal;
	}

	return deltaIncrease;
}

void fillVectorFromInputFile(std::vector<int>& vector) 
{
	std::ifstream inputFile;

	inputFile.open("input.txt");

	std::string line;

	while (std::getline(inputFile, line)) {
		vector.push_back(std::stoi(line));
	}
}

int main()
{
	std::vector<int> input;

	fillVectorFromInputFile(input);

	std::cout << getWindowIncreaseCount(input) << std::endl;

	return 0;
}