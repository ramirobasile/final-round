#include "level.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

fr::Level::Level(sf::FloatRect left, sf::FloatRect right, sf::Texture background)
		: left(left), right(right), background(background) {
}

std::vector<sf::FloatRect> fr::Level::getGeometry() const {
	return std::vector<sf::FloatRect> {left, right};
}

void fr::Level::draw(sf::RenderWindow &window) {
	window.draw(sf::Sprite(background));
}

///////////////////////////////////////////////////////////
// Debug methods
///////////////////////////////////////////////////////////
void fr::Level::drawDebugGeometry(sf::RenderWindow &window) {
	sf::RectangleShape left_shape(sf::Vector2f(left.width, left.height));
	left_shape.setPosition(sf::Vector2f(left.left, left.top));
	left_shape.setFillColor(sf::Color::Green);

	sf::RectangleShape right_shape(sf::Vector2f(right.width, right.height));
	right_shape.setPosition(sf::Vector2f(right.left, right.top));
	right_shape.setFillColor(sf::Color::Green);

	window.draw(left_shape);
	window.draw(right_shape);
}
