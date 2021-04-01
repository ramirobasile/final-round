#include "utils.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "main.hpp"

// This is mainly used to make logs more readable
std::string getGlobalTime() {
	std::stringstream stream;
	stream << std::setw(8) << std::setfill('0') << fr::global_time;
	return stream.str();
}

// Why cmath doesn't have this is beyond me
int sign(int n) {
	if (n > 0)
		return 1;
	if (n < 0)
		return -1;
	return 0;
}
