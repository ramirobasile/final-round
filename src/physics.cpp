#include "physics.hpp"

#include <cmath>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "utils.hpp"

void fr::updatePosition(sf::Vector2f &position, sf::Vector2f velocity,
		float dt) {
	position.x += velocity.x * dt;
}

void fr::resolveCollision(sf::Vector2f &position, sf::FloatRect bounds,
		sf::FloatRect obstacle) {
	sf::FloatRect overlap;

	if(bounds.intersects(obstacle, overlap)) {
		// Resolve by correcting the smallest overlap
		if (std::abs(overlap.width) < std::abs(overlap.height))
			position.x -= overlap.width * (overlap.left > bounds.left ? 1 : -1);
		else
			position.y -= overlap.height * (overlap.top > bounds.top ? 1 : -1);
	}
}

fr::Collision fr::getCollision(sf::FloatRect rect1, sf::FloatRect rect2) {
	Collision collision = Collision::none;
	sf::FloatRect overlap;

	if (rect1.intersects(rect2, overlap)) {
		if (rect1.left >= overlap.left)
			collision = Collision::left;

		if (rect1.left <= overlap.left)
			collision = Collision::right;

		if (rect1.top <= overlap.top)
			collision = Collision::top;

		if (rect1.top <= overlap.top)
			collision = Collision::bottom;
	}

	return collision;
}
