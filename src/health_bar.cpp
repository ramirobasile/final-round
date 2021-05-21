#include "health_bar.hpp"

#include "SFML/Graphics.hpp"
#include "direction.hpp"
#include "health.hpp"

fr::HealthBar::HealthBar(sf::Vector2f position, fr::Direction direction, 
		sf::Texture full, sf::Texture left_full, sf::Texture right_full, 
		sf::Texture empty, sf::Texture left_empty, sf::Texture right_empty)
		: position(position), direction(direction), full(full), 
		left_full(left_full), right_full(right_full), empty(empty),
		left_empty(left_empty), right_empty(right_empty) {
}

void fr::HealthBar::draw(sf::RenderTarget& window, fr::Health const& health) {
	// Draw empty
	for (int i = 0; i < health.getMax(); i+=2) {
		sf::Sprite sprite(empty);
		
		if (i == health.getMax() - 1) {
			if (direction == Direction::left)
				sprite = sf::Sprite(right_empty);
			else
				sprite = sf::Sprite(left_empty);
		}
			
		sprite.setPosition(
				position.x + (sprite.getTextureRect().width * (i / 2) + i) * (int)direction, 
				position.y);
		
		window.draw(sprite);
	}
	
	// Draw full
	for (int i = 0; i < health.getCurrent(); i+=2) {
		sf::Sprite sprite(full);
		
		if (i == health.getCurrent() - 1) {
			if (direction == Direction::left)
				sprite = sf::Sprite(right_full);
			else
				sprite = sf::Sprite(left_full);
		}
			
		sprite.setPosition(
				position.x + (sprite.getTextureRect().width * (i / 2) + i) * (int)direction,
				position.y);
		
		window.draw(sprite);
	}
}
