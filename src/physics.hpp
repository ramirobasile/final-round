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

void updatePosition(sf::FloatRect &bounds, sf::Vector2f velocity, float dt);
Collision getCollision(sf::FloatRect rect1, sf::FloatRect rect2);
void resolveCollision(sf::FloatRect &rect1, sf::FloatRect rect2);

} // namespace fr
