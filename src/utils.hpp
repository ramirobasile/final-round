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
inline int sign(double n) {
	int res = 0;
	
	if (n > 0)
		res = 1;
	else if (n < 0)
		res = -1;
		
	return res;
}

// Why SFML's Vector2 doesn't have this is beyond me
inline float distance(sf::Vector2f a, sf::Vector2f b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

const double PI = std::atan(1) * 4;
