#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

namespace fr {

class Level {
	public:
	Level();
	Level(sf::FloatRect left, sf::FloatRect right, sf::Texture background);

	void draw(sf::RenderWindow &window);
	void drawDebugGeometry(sf::RenderWindow &window);
	std::vector<sf::FloatRect> getGeometry();

	private:
	sf::FloatRect left;
	sf::FloatRect right;
	sf::Texture background;
};

} // namespace fr
