#include "player.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "main.hpp"
#include "utils.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "animation.hpp"
#include "stats.hpp"

fr::Player::Player(int index, int direction, fr::Device input_dev, 
		std::vector<int> controls, sf::Vector2f position, 
		sf::Texture spritesheet, std::vector<fr::Animation> animations, 
		fr::Stats stats)
		: index(index), direction(direction), input_dev(input_dev), 
		controls(controls), animations(animations), stats(stats) {
	bounds = sf::IntRect(position.x, position.y, 100, 100);
	//hurtbox = sf::IntRect(position.x, position.y, 100, 100);
	sprite.setTexture(spritesheet);
	sprite.setTextureRect(bounds);
}

void fr::Player::update(std::vector<sf::IntRect> geometry) {
	updateBuffer(buffer, buffer_ttl, inputs);
	if (input_dev == Device::keyboard)
		updateInputs(inputs, buffer, controls);
	else if (input_dev == Device::joystick)
		updateInputs(index, inputs, buffer, controls);

	state.update(inputs, buffer);

	updateVelocity(velocity, state, last_state, stats);
	updatePosition(bounds, velocity, geometry);

	// Debug
	if (false && state != last_state) {
		printTime(global_time);
		std::cout << "[P" << std::to_string(index) << "] " 
				<< state << std::endl << std::endl;
	}

	if (false && !inputs.empty()) {
		printTime(global_time);
		std::cout << "[P" << std::to_string(index) << "] Inputs:" << std::endl;
		for (int i = 0; i < inputs.size(); ++i)
			std::cout << "* " << inputs[i] << std::endl;
		std::cout << std::endl;
	}
}

void fr::Player::draw(sf::RenderWindow &window) {
	//Animation animaton = animations[animation];

	//sprite.setTextureRect(sf::IntRect(animaton.frame * animaton.width,
	//		animation * animaton.height, animaton.width, animaton.height));

	// Debug
	sf::RectangleShape bounds_shape(size());
	bounds_shape.setPosition(position());
	bounds_shape.setFillColor(sf::Color::Cyan);

	window.draw(bounds_shape);
}

sf::Vector2f fr::Player::position() {
	return sf::Vector2f(bounds.left, bounds.top);
}

sf::Vector2f fr::Player::size() {
	return sf::Vector2f(bounds.width, bounds.height);
}

void fr::Player::updateVelocity(sf::Vector2f &velocity, fr::State state,
		fr::State last_state, fr::Stats stats) {
	velocity.x = 0; // Linear velocity

	switch (state.movement) {
		case fr::Movement::walk_b:
			velocity.x = stats.walk_speed * -direction;
			break;

		case fr::Movement::walk_f:
			velocity.x = stats.walk_speed * direction;
			break;
	}
}
