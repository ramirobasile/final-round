#include "physics.hpp"

#include <cmath>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "utils.hpp"

void fr::updatePosition(sf::FloatRect &bounds, sf::Vector2f velocity, float dt) {
	bounds.left += velocity.x * dt;
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

void fr::resolveCollision(sf::FloatRect &rect1, sf::FloatRect rect2) {
	sf::FloatRect overlap;

	if(rect1.intersects(rect2, overlap)) {
		// Always attempt to resolve by correcting the smallest overlap
		if (std::abs(overlap.width) < std::abs(overlap.height))
			rect1.left -= overlap.width * (overlap.left > rect1.left ? 1 : -1);
		else
			rect1.top -= overlap.height * (overlap.top > rect1.top ? 1 : -1);
	}
}
