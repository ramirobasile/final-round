#pragma once

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

namespace fr {

struct Stats {
	int max_health = 20;
	float health_regen = 1;
	float regen_rate = 1;

	float walk_speed = 92;
	
	sf::Vector2f bounds = sf::Vector2f(32, 64);
	sf::FloatRect head_hurtbox = sf::FloatRect(16, 3, 16, 16);
	sf::FloatRect body_hurtbox = sf::FloatRect(12, 19, 16, 16);
};

} // namespace fr
