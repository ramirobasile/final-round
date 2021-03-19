#pragma once

#include <iostream>
#include <iomanip>

inline void printTime(int time) {
	std::cout << "[" << std::setw(8) << std::setfill('0') << time << "] ";
}

inline int sign(int n) {
	if (n > 0) 
		return 1;
	if (n < 0) 
		return -1;
	return 0;
}
