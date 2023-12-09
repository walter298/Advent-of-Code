#include "day6.h"

using Num = long long;

using Race = std::pair<Num, Num>;
using Races = std::array<Race, 4>;

Num raceDist(Num timeHeld, Num raceLen) {
	return timeHeld * (raceLen - timeHeld);
}

Num betterRacesC(const Race& r) {
	const auto& [raceLen, recordDist] = r;

	Num timeHeld = raceLen / 2;
	
	Num betterRunsC = 0;
	while (raceDist(timeHeld, raceLen) > recordDist) {
		betterRunsC++;
		timeHeld--;
	}
	
	return (raceLen % 2 == 0) ? betterRunsC * 2 - 1 : betterRunsC * 2;
}

Races getTrials() {
	Races ret;

	std::ifstream file{ advent::workingDirectory + "/2023/inputs/day6.txt" };
	assert(file.is_open());

	std::istringstream iss;
	std::string line;

	auto readNums = [&](auto memp) {
		std::getline(file, line);
		iss.str(line);
		iss >> line >> ret[0].*memp >> ret[1].*memp >> ret[2].*memp >> ret[3].*memp;
		iss.clear();	
	};

	readNums(&Races::value_type::first);
	readNums(&Races::value_type::second);

	file.close();

	return ret;
}

Race getTrialPart2() {
	Race ret;

	std::ifstream file{ advent::workingDirectory + "/2023/inputs/day6.txt" };
	assert(file.is_open());

	std::istringstream iss;
	std::string line;
	
	auto readNum = [&](auto memp) {
		std::getline(file, line);
		iss.str(line);
		std::string a, b, c, d;
		iss >> line >> a >> b >> c >> d;
		std::cout << a + b + c + d << '\n';
		ret.*memp = std::stoll(a + b + c + d);
		iss.clear();
	};
	readNum(&Race::first);
	readNum(&Race::second);

	return ret;
}

void runDay6() {
	//Num prod = 1;
	/*auto trials = getTrials();
		for (const auto& race : trials) {
			prod *= betterRacesC(race);
		}*/
	//std::cout << prod << std::endl;

	auto trial = getTrialPart2();
	std::cout << betterRacesC(trial) << '\n';
}