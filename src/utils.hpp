#pragma once

#include <iostream>
#include <iomanip>

#include "main.hpp"

inline void printGlobalTime() {
	std::cout << "[" << std::setw(8) << std::setfill('0') << fr::global_time
			<< "] ";
}

inline int sign(int n) {
	if (n > 0) 
		return 1;
	if (n < 0) 
		return -1;
	return 0;
}
