#pragma once

#include <iostream>
#include <fstream>

#include <toml++/toml.h>

namespace fr {
	extern int global_time; // In milliseconds
	extern float dt;
	extern toml::table config;
}
