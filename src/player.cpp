#include "player.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "punch.hpp"
#include "stats.hpp"
#include "sprite.hpp"
#include "utils.hpp"

fr::Player::Player() {} // Empty constructor

fr::Player::Player(int index, std::string alias, int direction, 
		fr::Device input_dev, std::vector<int> controls, sf::Vector2f position, 
		fr::Sprite sprite, fr::Stats stats)
		: index(index), alias(alias), direction(direction), 
		input_dev(input_dev), controls(controls), sprite(sprite), stats(stats) {
	bounds = sf::IntRect(position.x, position.y, 32, 64);
	head_hurtbox = sf::IntRect(20, 3, 16, 16);
	body_hurtbox = sf::IntRect(16, 19, 16, 16);

	punches = default_punches;

	max_health = stats.max_health;
	health = max_health;
}

void fr::Player::update(float dt, int global_time,
		std::vector<sf::IntRect> geometry, fr::Player &opponent) {
	// Input
	updateBuffer(buffer, buffer_ttl, inputs, dt);
	if (input_dev == Device::keyboard)
		updateInputs(inputs, buffer, buffer_ttl, controls, dt);
	else if (input_dev == Device::joystick)
		updateInputs(index, inputs, buffer, buffer_ttl, controls, dt);

	// State
	last_state = state;
	state.update(inputs, buffer, punches, dt);

	if (!state.isPunching() && !state.isGuarding())
		tt_regen += dt;
	
	if (tt_regen > stats.regen_rate) {
		takeDamage(-stats.health_regen);
		tt_regen = 0;
	}

	// Interrupt punches when hurtbox obstructed
	bool clear = !getHeadHurtbox().intersects(opponent.getHeadHurtbox())
			&& !getBodyHurtbox().intersects(opponent.getBodyHurtbox());
	if (state.punch.canInterrupt() && state.punch.needs_clear && !clear)
		state.punch.interrupt();

	if (last_state.punch.canInterrupt() && !state.punch.canInterrupt())
		takeDamage(state.punch.self_damage);

	// Hit opponent
	sf::IntRect hitbox = state.punch.getHitbox(bounds, direction);
	if (state.punch.isActive()) {
		if (hitbox.intersects(opponent.getBodyHurtbox()))
			opponent.takeHit(state.punch, false);
		else if (hitbox.intersects(opponent.getHeadHurtbox()))
			opponent.takeHit(state.punch, true);
	}

	// Sprite
	sprite.update(state, last_state, dt);

	// Physics
	updateVelocity(velocity, state, last_state, stats);
	updatePosition(bounds, velocity, dt);
	resolveCollision(bounds, opponent.bounds);
	for (int i = 0; i < geometry.size(); ++i)
		resolveCollision(bounds, geometry[i]);
}

void fr::Player::draw(sf::RenderWindow &window) {
	sprite.draw(window, bounds, direction);
}

void fr::Player::takeDamage(int damage) {
	health = std::clamp(health - damage, 0, max_health);
}

void fr::Player::takePermaDamage(int damage) {
	max_health = std::clamp(max_health - damage, 0, stats.max_health);
}

void fr::Player::takeHit(fr::Punch &punch, bool head) {
	if (state.guard_high && head || state.guard_low) {
		takeDamage(punch.block_damage);
	} else {
		takeDamage(punch.damage);
		takePermaDamage(punch.perma_damage);
	}
	punch.interrupt();
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
		left = bounds.left - head_hurtbox.left + bounds.width - head_hurtbox.width;
	int top = bounds.top + head_hurtbox.top;
	return sf::IntRect(left, top, head_hurtbox.width, head_hurtbox.height);
}

sf::IntRect fr::Player::getBodyHurtbox() const {
	int left;
	if (direction == 1)
		left = bounds.left + body_hurtbox.left;
	else
		left = bounds.left - body_hurtbox.left + bounds.width - body_hurtbox.width;
	int top = bounds.top + body_hurtbox.top;
	return sf::IntRect(left, top, body_hurtbox.width, body_hurtbox.height);
}

void fr::Player::updateVelocity(sf::Vector2f &velocity, fr::State state,
		fr::State last_state, fr::Stats stats) {
	velocity.x = 0; // Linear velocity

	switch (state.movement) {
		case fr::Movement::walk_l:
			if (!state.isPunching() || state.punch.canInterrupt())
				velocity.x = -stats.walk_speed;
			break;

		case fr::Movement::walk_r:
			if (!state.isPunching() || state.punch.canInterrupt())
				velocity.x = stats.walk_speed;
			break;
	}
}

///////////////////////////////////////////////////////////
// Debug methods
///////////////////////////////////////////////////////////
void fr::Player::logState(int global_time) {
	std::string pre = "[" + fillString(std::to_string(global_time), 8, '0')
			+ "] [P" +  std::to_string(index) + "] ";

	if (state.movement != last_state.movement) {
		std::cout << pre + "Movement: " + std::to_string((int)state.movement)
				<< std::endl;
	}
	if (state.isPunching()) {
		std::cout << pre + "Punch: " + std::to_string(state.punch.progress)
				<< std::endl;
	}
}

void fr::Player::logInputs(int global_time) {
	std::string pre = "[" + fillString(std::to_string(global_time), 8, '0')
			+ "] [P" +  std::to_string(index) + "] ";

	if (!inputs.empty()) {
		std::cout << pre + "Inputs:" << std::endl;
		for (int i = 0; i < inputs.size(); ++i)
			std::cout << "* " + (std::string)inputs[i] << std::endl;
	}
}

void fr::Player::drawDebugGeometry(sf::RenderWindow &window) {
	sf::RectangleShape shape(getSize());
	shape.setPosition(getPosition());
	shape.setFillColor(sf::Color::Cyan);
	window.draw(shape);
}

void fr::Player::drawDebugHurtboxes(sf::RenderWindow &window) {
	sf::RectangleShape head(sf::Vector2f(getHeadHurtbox().width, 
			getHeadHurtbox().height));
	head.setPosition(getHeadHurtbox().left, getHeadHurtbox().top);
	head.setFillColor(sf::Color::Yellow);
	window.draw(head);

	sf::RectangleShape body(sf::Vector2f(getBodyHurtbox().width, 
			getBodyHurtbox().height));
	body.setPosition(getBodyHurtbox().left, getBodyHurtbox().top);
	body.setFillColor(sf::Color::Yellow);
	window.draw(body);
}

void fr::Player::drawDebugHitboxes(sf::RenderWindow &window) {
	if (state.isPunching() && state.punch.isActive()) {
		sf::IntRect hitbox = state.punch.getHitbox(bounds, direction);
		sf::RectangleShape shape(sf::Vector2f(hitbox.width, hitbox.height));
		shape.setPosition(hitbox.left, hitbox.top);
		shape.setFillColor(sf::Color::Red);
		window.draw(shape);
	}
}
