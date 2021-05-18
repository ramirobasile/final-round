#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

// Why SFML's Vector2 doesn't have this is beyond me
inline float vector2Distance(sf::Vector2f a, sf::Vector2f b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}
