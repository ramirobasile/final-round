#include "player.hpp"

#include <iostream>
#include <vector>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "config.hpp"
#include "main.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "animation.hpp"
#include "punch.hpp"
#include "stats.hpp"
#include "utils.hpp"

fr::Player::Player(int index, int direction, fr::Device input_dev, 
		std::vector<int> controls, sf::Vector2f position, 
		sf::Texture spritesheet, std::vector<fr::Animation> animations,
		fr::Stats stats)
		: index(index), direction(direction), input_dev(input_dev), 
		controls(controls), animations(animations), stats(stats) {
	health = stats.max_health;
	bounds = sf::IntRect(position.x, position.y, 100, 100);
	head_hurtbox = sf::IntRect(position.x, position.y, 100, 20);
	body_hurtbox = sf::IntRect(position.x, position.y + 20, 100, 80);
	sprite.setTexture(spritesheet);
	sprite.setTextureRect(bounds);
	punches = default_punches;
}

void fr::Player::update(std::vector<sf::IntRect> geometry, fr::Player opponent) {
	// Input
	updateBuffer(buffer, buffer_ttl, inputs);
	if (input_dev == Device::keyboard)
		updateInputs(inputs, buffer, buffer_ttl, controls);
	else if (input_dev == Device::joystick)
		updateInputs(index, inputs, buffer, buffer_ttl, controls);

	// State
	last_state = state;
	state.update(inputs, buffer, punches);

	sf::IntRect clearbox = state.punch.getClearbox(position(), direction);
	bool obstructed = clearbox.intersects(opponent.headHurtbox())
			|| clearbox.intersects(opponent.bodyHurtbox());
	if (state.punch.interruptible() && obstructed)
		state.punch.interrupt();

	bool was_int = last_state.punch.interruptible();
	if (was_int && !state.punch.interruptible())
		takeDamage(state.punch.self_damage);

	sf::IntRect hitbox = state.punch.getHitbox(position(), direction);
	if (state.punch.active() && hitbox.intersects(opponent.headHurtbox())) {
		opponent.takeHeadHit(state.punch);
		state.punch.interrupt();
	}

	if (state.punch.active() && hitbox.intersects(opponent.bodyHurtbox())) {
		opponent.takeBodyHit(state.punch);
		state.punch.interrupt();
	}

	// Physics
	updateVelocity(velocity, state, last_state, stats);
	updatePosition(bounds, velocity);
	resolveCollision(bounds, opponent.bounds);
	for (int i = 0; i < geometry.size(); ++i)
		resolveCollision(bounds, geometry[i]);

	// Debug
	if (config.getBool("debug", "log_state", false)
			&& state != last_state) {
		printGlobalTime();
		std::cout << "[P" << std::to_string(index) << "] ";
		std::cout<< state << std::endl << std::endl;
	}

	if (config.getBool("debug", "log_inputs", false)
			&& !inputs.empty()) {
		printGlobalTime();
		std::cout << "[P" << std::to_string(index) << "] ";
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
	if (config.getBool("debug", "draw_geometry", false)) {
		sf::RectangleShape shape(size());
		shape.setPosition(position());
		shape.setFillColor(sf::Color::Cyan);

		window.draw(shape);
	}

	/*if (config.getBool("debug", "draw_hitboxes", false)
			&& state.isPunching()) {
		sf::Color color;
		if (state.punch.isStartingUp(state.punch_progress))
			color = sf::Color::Yellow;
		if (state.punch.isActive(state.punch_progress))
			color = sf::Color::Red;

		sf::IntRect hitbox = state.punch.getHitbox(getPosition());
		sf::RectangleShape shape(sf::Vector2f(hitbox.width, hitbox.height));
		shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
		shape.setFillColor(color);

		window.draw(shape);
	}*/
}

// TODO Take damage and hits
void fr::Player::takeDamage(int damage) {
}

void fr::Player::takeHeadHit(fr::Punch punch) {
}

void fr::Player::takeBodyHit(fr::Punch punch) {
}

sf::Vector2f fr::Player::position() const {
	return sf::Vector2f(bounds.left, bounds.top);
}

sf::Vector2f fr::Player::size() const {
	return sf::Vector2f(bounds.width, bounds.height);
}

sf::IntRect fr::Player::headHurtbox() const {
	int left;
	if (direction == 1)
		left = bounds.left + head_hurtbox.left;
	else
		left = bounds.width - head_hurtbox.left;
	int top = bounds.top + head_hurtbox.top;
	return sf::IntRect(top, left, head_hurtbox.width, head_hurtbox.height);
}

sf::IntRect fr::Player::bodyHurtbox() const {
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

	// TODO Don't walk when punch is between unstoppable and hitbox_end
	switch (state.movement) {
		case fr::Movement::walk_l:
			if ((state.punching() && !state.punch.interruptible()))
				break;

			velocity.x = -stats.walk_speed;
			break;

		case fr::Movement::walk_r:
			if ((state.punching() && !state.punch.interruptible()))
				break;

			velocity.x = stats.walk_speed;
			break;
	}
}
