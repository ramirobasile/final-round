#include "utils.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

#include "main.hpp"

// This is mainly used to make logs more readable
void printGlobalTime() {
	std::cout << "[" << std::setw(8) << std::setfill('0') << fr::global_time
			<< "] ";
}

// Why cmath doesn't have this is beyond me
int sign(int n) {
	if (n > 0)
		return 1;
	if (n < 0)
		return -1;
	return 0;
}
