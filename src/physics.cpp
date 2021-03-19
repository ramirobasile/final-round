#include "physics.hpp"

#include <vector>
#include <cmath>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "main.hpp"
#include "utils.hpp"

void fr::updatePosition(sf::IntRect &bounds, sf::Vector2f velocity,
		std::vector<sf::IntRect> geometry) {
	// Random math necessary to avoid weird rounding errors
	bounds.left += sign(velocity.x) * (int)std::abs(velocity.x * dt);
	
	for (int i = 0; i < geometry.size(); ++i)
		resolveCollision(bounds, geometry[i]);
}

fr::Collision fr::getCollision(sf::IntRect rect1, sf::IntRect rect2) {
	Collision collision = Collision::none;
	sf::IntRect overlap;

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

void fr::resolveCollision(sf::IntRect &rect1, sf::IntRect rect2) {
	sf::IntRect overlap;

	if(rect1.intersects(rect2, overlap)) {
		// Always attempt to resolve by correcting the smallest overlap
		if (std::abs(overlap.width) < std::abs(overlap.height))
			rect1.left -= overlap.width * (overlap.left > rect1.left ? 1 : -1);
		else
			rect1.top -= overlap.height * (overlap.top > rect1.top ? 1 : -1);
	}
}
