#include <iostream>
#include <fstream>
#include <string>
#include <vector>

double binaryToDecimal(std::string b)
{
	double d = 0;

	double digitAdder;

	for (int i = 11; i >= 0; i--) {
		digitAdder = pow(2, b.size() - i - 1);

		int currentDigit = b[i] - '0';

		d += currentDigit * digitAdder;
	}

	return d;
}

std::vector<std::string> trimmedVector(std::vector<std::string> vector, int index, bool findingOxygen)
{
	std::vector<std::string> zeros, ones;

	if (vector.size() == 1) {
		return vector;
	}

	for (std::string& s : vector) {
		if (s[index] == '0') {
			zeros.push_back(s);
		} else {
			ones.push_back(s);
		}
	}

	if (zeros.size() > ones.size()) {
		if (findingOxygen) {
			return zeros;
		} else {
			return ones;
		}
	} else {
		if (findingOxygen) {
			return ones;
		} else {
			return zeros;
		}
	} 
}

void getOAndCO2Rates(double &oxygen, double &CO2)
{
	std::vector<std::string> oxygenRateCanidates, CO2RateCanidates, zeros, ones;

	std::ifstream inputFile;
	inputFile.open("input.txt");

	std::string line;

	while (std::getline(inputFile, line)) {
		if (line[0] == '0') {
			zeros.push_back(line);
		} else {
			ones.push_back(line);
		}
	}

	if (ones.size() > zeros.size()) {
		oxygenRateCanidates = ones;
		CO2RateCanidates = zeros;
	} else if (zeros.size() > ones.size()) {
		oxygenRateCanidates = zeros;
		CO2RateCanidates = ones;
	} else {
		oxygenRateCanidates = ones;
		CO2RateCanidates = zeros;
	}
		 
	for (int i = 1; i <= 11; i++) {
		oxygenRateCanidates = trimmedVector(oxygenRateCanidates, i, true);
		CO2RateCanidates = trimmedVector(CO2RateCanidates, i, false);
	}

	oxygen = binaryToDecimal(oxygenRateCanidates[0]);
	CO2 = binaryToDecimal(CO2RateCanidates[0]);
}

void getGammaAndEpsilonRates(double& gamma, double& epsilon)
{
	std::string gammaBinary, epsilonBinary;

	std::string line;

	int zeroCount, oneCount;

	for (int i = 0; i <= 11; i++) {
		std::ifstream inputFile;
		inputFile.open("input.txt");

		zeroCount = 0, oneCount = 0;

		line.clear();

		while (std::getline(inputFile, line)) {
			if (line[i] == '0') {
				zeroCount++;
			} else {
			    oneCount++;
			}
		}

		if (zeroCount > oneCount) {
			gammaBinary.push_back('0');
			epsilonBinary.push_back('1');
		} else {
			gammaBinary.push_back('1');
			epsilonBinary.push_back('0');
		}
	}

	gamma = binaryToDecimal(gammaBinary);
	epsilon = binaryToDecimal(epsilonBinary);
}

int main()
{
	double gamma = 0, epsilon = 0;
	double o = 0, co2 = 0;

	getOAndCO2Rates(o, co2);

	std::cout << o << ", " << co2 << std::endl;
}