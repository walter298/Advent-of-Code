#include <iostream>
#include <fstream>
#include <string>

void getDepthAndDistanceAndAim(int &distance, int &depth, int &aim)
{
	std::ifstream inputFile;
	inputFile.open("input.txt");

	std::string line;

	while (std::getline(inputFile, line)) {
		switch (line[0]) {
		case 'u':
			aim -= line[3] - '0';
			break;
		case 'd':
			aim += line[5] - '0';
			break;
		case 'f':
			distance += line[8] - '0';
			depth += aim * (line[8] - '0');

			break;
		}
	}
}

int main()
{
	int depth = 0, distance = 0, aim = 0;

	getDepthAndDistanceAndAim(depth, distance, aim);

	std::cout << depth * distance << std::endl;
}