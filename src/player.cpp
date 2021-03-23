#include "player.hpp"

#include <iostream>
#include <vector>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "main.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "animation.hpp"
#include "stats.hpp"
#include "utils.hpp"

fr::Player::Player(int index, int direction, fr::Device input_dev, 
		std::vector<int> controls, sf::Vector2f position, 
		sf::Texture spritesheet, std::vector<fr::Animation> animations, 
		std::vector<fr::Punch> punches, fr::Stats stats)
		: index(index), direction(direction), input_dev(input_dev), 
		controls(controls), animations(animations), punches(punches),
		stats(stats) {
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

	state.update(inputs, buffer, punches);

	updateVelocity(velocity, state, last_state, stats);
	updatePosition(bounds, velocity, geometry);

	// Debug
	if (config["debug"]["log_state"].value_or(false) && state != last_state) {
		printGlobalTime();
		std::cout << "[P" << std::to_string(index + 1) << "] ";
		std::cout<< state << std::endl << std::endl;
	}

	if (config["debug"]["log_inputs"].value_or(false) && !inputs.empty()) {
		printGlobalTime();
		std::cout << "[P" << std::to_string(index + 1) << "] ";
		std::cout << "Inputs:" << std::endl;
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
	if (config["debug"]["draw_geometry"].value_or(false)) {
		sf::RectangleShape shape(getSize());
		shape.setPosition(getPosition());
		shape.setFillColor(sf::Color::Cyan);

		window.draw(shape);
	}

	if (config["debug"]["draw_hitboxes"].value_or(false) && state.isPunching()
			&& state.punch.isActive(state.punch_progress)) {
		//sf::RectangleShape shape(sf::Vector2f(hitbox.width, hitbox.height));
		//shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
		//shape.setFillColor(sf::Color::Red);

		//window.draw(shape);
	}
}

sf::Vector2f fr::Player::getPosition() const {
	return sf::Vector2f(bounds.left, bounds.top);
}

sf::Vector2f fr::Player::getSize() const {
	return sf::Vector2f(bounds.width, bounds.height);
}

sf::IntRect fr::Player::getHeadHurtbox() const {
	int left;
	if (direction == 1)
		left = bounds.left + head_hurtbox.left;
	else
		left = bounds.width - head_hurtbox.left;
	int top = bounds.top + head_hurtbox.top;
	return sf::IntRect(top, left, head_hurtbox.width, head_hurtbox.height);
}

sf::IntRect fr::Player::getBodyHurtbox() const {
	int left;
	if (direction == 1)
		left = bounds.left + body_hurtbox.left;
	else
		left = bounds.width - body_hurtbox.left;
	int top = bounds.top + body_hurtbox.top;
	return sf::IntRect(top, left, body_hurtbox.width, body_hurtbox.height);
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
