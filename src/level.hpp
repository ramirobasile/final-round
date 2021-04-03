#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

namespace fr {

struct Level {
	std::vector<sf::IntRect> geometry();
	void draw(sf::RenderWindow &window);
	void drawDebugGeometry(sf::RenderWindow &window);

	sf::IntRect left;
	sf::IntRect right;
	sf::Texture background;
};

} // namespace fr
