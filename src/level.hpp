#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

namespace fr {

class Level {
	public:
	Level();
	Level(sf::IntRect left, sf::IntRect right, sf::Texture background);

	void draw(sf::RenderWindow &window);
	void drawDebugGeometry(sf::RenderWindow &window);
	std::vector<sf::IntRect> getGeometry();

	private:
	sf::IntRect left;
	sf::IntRect right;
	sf::Texture background;
};

} // namespace fr
