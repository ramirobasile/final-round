#include "player.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "animation.hpp"
#include "punch.hpp"
#include "stats.hpp"
#include "utils.hpp"

float regen_timer = 0;

// Empty constructor
fr::Player::Player() {
}

fr::Player::Player(int index, int direction, fr::Device input_dev,
		std::vector<int> controls, sf::Vector2f position, 
		sf::Texture spritesheet, std::vector<fr::Animation> animations,
		fr::Stats stats)
		: index(index), direction(direction), input_dev(input_dev), 
		controls(controls), animations(animations), stats(stats) {
	bounds = sf::IntRect(position.x, position.y, 104, 128);
	head_hurtbox = sf::IntRect(64, 0, 32, 32);
	body_hurtbox = sf::IntRect(48, 32, 32, 32);

	sprite.setTexture(spritesheet);
	sprite.setTextureRect(bounds);

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

	regen_timer += dt;
	if (!state.punching() && !state.guarding() && regen_timer > stats.regen_rate) {
		takeDamage(-stats.health_regen);
		regen_timer = 0;
	}

	// Interrupt punches when obstructed
	sf::IntRect clearbox = state.punch.getClearbox(bounds, direction);
	bool obstructed = clearbox.intersects(opponent.headHurtbox())
			|| clearbox.intersects(opponent.bodyHurtbox());
	if (state.punch.interruptible() && obstructed)
		state.punch.interrupt();

	if (last_state.punch.interruptible() && !state.punch.interruptible())
		takeDamage(state.punch.self_damage);

	// Hit opponent
	sf::IntRect hitbox = state.punch.getHitbox(bounds, direction);
	if (state.punch.active()) {
		if (hitbox.intersects(opponent.bodyHurtbox()))
			opponent.takeHit(state.punch, false);
		else if (hitbox.intersects(opponent.headHurtbox()))
			opponent.takeHit(state.punch, true);
	}

	// Physics
	updateVelocity(velocity, state, last_state, stats);
	updatePosition(bounds, velocity, dt);
	resolveCollision(bounds, opponent.bounds);
	for (int i = 0; i < geometry.size(); ++i)
		resolveCollision(bounds, geometry[i]);
}

void fr::Player::draw(sf::RenderWindow &window) {
	//Animation animaton = animations[animation];

	//sprite.setTextureRect(sf::IntRect(animaton.frame * animaton.width,
	//		animation * animaton.height, animaton.width, animaton.height));
}

void fr::Player::takeDamage(int damage) {
	health = std::clamp(health - damage, 0, max_health);
}

void fr::Player::takePermaDamage(int damage) {
	max_health = std::clamp(max_health - damage, 0, stats.max_health);
}

void fr::Player::takeHit(fr::Punch punch, bool head) {
	if (state.guard_high && head || state.guard_low) {
		takeDamage(punch.block_damage);
	} else {
		takeDamage(punch.damage);
		takePermaDamage(punch.perma_damage);
	}
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
		left = bounds.left - head_hurtbox.left + bounds.width - head_hurtbox.width;
	int top = bounds.top + head_hurtbox.top;
	return sf::IntRect(left, top, head_hurtbox.width, head_hurtbox.height);
}

sf::IntRect fr::Player::bodyHurtbox() const {
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
			if (!state.punching() || state.punch.interruptible())
				velocity.x = -stats.walk_speed;
			break;

		case fr::Movement::walk_r:
			if (!state.punching() || state.punch.interruptible())
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

	if (state.movement != last_state.movement)
		std::cout << pre + "Movement: " + std::to_string((int)state.movement) << std::endl;
	if (state.punching())
		std::cout << pre + "Punch: " + std::to_string(state.punch.progress) << std::endl;
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
	sf::RectangleShape shape(size());
	shape.setPosition(position());
	shape.setFillColor(sf::Color::Cyan);
	window.draw(shape);
}

void fr::Player::drawDebugHurtboxes(sf::RenderWindow &window) {
	sf::RectangleShape head(sf::Vector2f(headHurtbox().width, headHurtbox().height));
	head.setPosition(headHurtbox().left, headHurtbox().top);
	head.setFillColor(sf::Color::Yellow);
	window.draw(head);

	sf::RectangleShape body(sf::Vector2f(bodyHurtbox().width, bodyHurtbox().height));
	body.setPosition(bodyHurtbox().left, bodyHurtbox().top);
	body.setFillColor(sf::Color::Yellow);
	window.draw(body);
}

void fr::Player::drawDebugHitboxes(sf::RenderWindow &window) {
	if (state.punching()) {
		sf::Color color = sf::Color::Blue;
		if (state.punch.active())
			color = sf::Color::Red;

		sf::IntRect hitbox = state.punch.getHitbox(bounds, direction);
		sf::RectangleShape shape(sf::Vector2f(hitbox.width, hitbox.height));
		shape.setPosition(hitbox.left, hitbox.top);
		shape.setFillColor(color);
		window.draw(shape);
	}
}
