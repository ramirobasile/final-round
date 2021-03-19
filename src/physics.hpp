#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "utils.hpp"

namespace fr {
	enum class Collision {
		none,
		left,
		right,
		top,
		bottom
	};

	void updatePosition(float dt, sf::IntRect &bounds, sf::Vector2f velocity, 
			std::vector<sf::IntRect> geometry);
	Collision getCollision(sf::IntRect rect1, sf::IntRect rect2);
	void resolveCollision(sf::IntRect &rect1, sf::IntRect rect2);
}
