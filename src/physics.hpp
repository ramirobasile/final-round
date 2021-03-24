#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

namespace fr {

enum class Collision {
	none,
	left,
	right,
	top,
	bottom
};

void updatePosition(sf::IntRect &bounds, sf::Vector2f velocity,
		std::vector<sf::IntRect> geometry);
Collision getCollision(sf::IntRect rect1, sf::IntRect rect2);
void resolveCollision(sf::IntRect &rect1, sf::IntRect rect2);

} // namespace fr
