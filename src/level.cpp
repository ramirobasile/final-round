#include "level.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "config.hpp"
#include "main.hpp"
		
std::vector<sf::IntRect> fr::Level::geometry() {
	return std::vector<sf::IntRect> {left, right};
}

void fr::Level::draw(sf::RenderWindow &window) {
	window.draw(sf::Sprite(background));

	// Debug
	if (config.getBool("debug", "draw_geometry", false)) {
		sf::RectangleShape left_shape(sf::Vector2f(left.width, left.height));
		left_shape.setPosition(sf::Vector2f(left.left, left.top));
		left_shape.setFillColor(sf::Color::Green);

		sf::RectangleShape right_shape(sf::Vector2f(right.width, right.height));
		right_shape.setPosition(sf::Vector2f(right.left, right.top));
		right_shape.setFillColor(sf::Color::Green);

		window.draw(left_shape);
		window.draw(right_shape);
	}
}
