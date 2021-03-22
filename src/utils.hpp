#pragma once

#include <iostream>
#include <iomanip>

#include <toml++/toml.h>
#include "main.hpp"
#include "punch.hpp"

// TODO Implement in cpp
inline void printGlobalTime() {
	std::cout << "[" << std::setw(8) << std::setfill('0') << fr::global_time
			<< "] ";
}

// TODO Implement in cpp
inline int sign(int n) {
	if (n > 0) 
		return 1;
	if (n < 0) 
		return -1;
	return 0;
}

std::vector<fr::Punch> getPunches(toml::table table);
fr::Punch parsePunch(toml::table punch);
