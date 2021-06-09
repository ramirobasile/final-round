#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

namespace fr {

struct Stats {
	int max_health = -1;
	int min_health = -1;
	int health_regen = -1;
	float regen_rate = -1;
	float velocity = -1;
	sf::FloatRect bounds;
};

} // namespace fr
