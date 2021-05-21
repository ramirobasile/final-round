#pragma once

#include "SFML/Graphics.hpp"
#include "direction.hpp"
#include "health.hpp"

namespace fr {

class HealthBar {
	public:
	HealthBar() {}; // Empty
	HealthBar(sf::Vector2f position, Direction direction, sf::Texture full, 
			sf::Texture left_full, sf::Texture right_full, sf::Texture empty, 
			sf::Texture left_empty, sf::Texture right_empty);
			
	void draw(sf::RenderTarget &window, Health const& health);
	
	private:
	sf::Vector2f position;
	Direction direction;
	sf::Texture ring;
	sf::Texture full;
	sf::Texture left_full;
	sf::Texture right_full;
	sf::Texture empty;
	sf::Texture left_empty;
	sf::Texture right_empty;
};

} // namespace fr
