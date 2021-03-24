#pragma once

#include <iostream>
#include <fstream>

#include "inih/cpp/INIReader.h"

namespace fr {
	extern int global_time; // In milliseconds
	extern float dt;
	extern INIReader config;

	const float FPS = 60;
}
