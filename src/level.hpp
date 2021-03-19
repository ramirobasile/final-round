#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

namespace fr {
	struct Level {
		sf::IntRect left;
		sf::IntRect right;
		
		std::vector<sf::IntRect> geometry();
		void draw(sf::RenderWindow &window);
	};
}
