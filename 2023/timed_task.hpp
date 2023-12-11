#include <concepts>
#include <chrono>
#include <iostream>

template<typename Time = std::chrono::milliseconds>
void time(std::invocable auto&& func) {
	auto start = std::chrono::steady_clock::now();
	func();
	std::cout << "task took " << 
		std::chrono::duration_cast<Time>(std::chrono::steady_clock::now() - start)
		<< std::endl;
}