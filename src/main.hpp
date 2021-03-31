#pragma once

#include "SFML/Graphics.hpp"
#include "config.hpp"

namespace fr {

extern int global_time; // In milliseconds
extern float dt;
extern ConfigFile config;
extern sf::Font font;

const float FPS = 60;

} // namespace fr
