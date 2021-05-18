#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

namespace fr {

enum class Collision {
	none,
	left,
	right,
	top,
	bottom,
};

void updatePosition(sf::Vector2f &position, sf::Vector2f velocity, float dt);
void resolveCollision(sf::Vector2f &position, sf::FloatRect bounds,
		sf::FloatRect obstable);
Collision getCollision(sf::FloatRect rect1, sf::FloatRect rect2);

} // namespace fr
