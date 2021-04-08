#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

inline std::string fillString(std::string str, int w, char fill) {
	std::stringstream stream;
	stream << std::setw(w) << std::setfill(fill) << str;
	return stream.str();
}

inline std::string changePresicion(float number, int presicion) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(presicion) << number;
	return stream.str();
}

// Why cmath doesn't have this is beyond me
inline int sign(int n) {
	if (n > 0)
		return 1;
	if (n < 0)
		return -1;
	return 0;
}
