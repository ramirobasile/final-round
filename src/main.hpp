#pragma once

#include <iostream>
#include <fstream>

#include <toml++/toml.h>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "match.hpp"

namespace fr {
	extern int global_time; // In milliseconds
	extern float dt;
	extern toml::table config;
}
