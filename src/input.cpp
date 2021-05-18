#include "input.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

fr::Input::operator std::string() const {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << held;

	return std::to_string((int)control) + " " + std::to_string((int)action)
			+ " " + stream.str();
}
