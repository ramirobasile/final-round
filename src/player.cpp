#include "player.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

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
	bounds = sf::IntRect(position.x, position.y, 64, 64);
	head_hurtbox = sf::IntRect(0, 0, 64, 24);
	body_hurtbox = sf::IntRect(0, 24, 48, 40);
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
		std::cout << "[P" << std::to_string(index) << "] State: ";
		std::cout<< state << std::endl << std::endl;
	}

	if (config.getBool("debug", "log_inputs", false)
			&& !inputs.empty()) {
		printGlobalTime();
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
	if (config.getBool("debug", "draw_geometry", false)) {
		sf::RectangleShape shape(size());
		shape.setPosition(position());
		shape.setFillColor(sf::Color::Cyan);
		window.draw(shape);
	}

	if (config.getBool("debug", "draw_hurtboxes", false)) {
		sf::RectangleShape head(sf::Vector2f(headHurtbox().width, headHurtbox().height));
		head.setPosition(sf::Vector2f(headHurtbox().left, headHurtbox().top));
		head.setFillColor(sf::Color::Yellow);
		window.draw(head);

		sf::RectangleShape body(sf::Vector2f(bodyHurtbox().width, bodyHurtbox().height));
		body.setPosition(sf::Vector2f(bodyHurtbox().left, bodyHurtbox().top));
		body.setFillColor(sf::Color::Yellow);
		window.draw(body);
	}

	if (config.getBool("debug", "draw_hitboxes", false) && state.punching()) {
		sf::Color color = sf::Color::Blue;
		if (state.punch.active())
			color = sf::Color::Red;

		sf::IntRect hitbox = state.punch.getHitbox(position(), direction);
		sf::RectangleShape shape(sf::Vector2f(hitbox.width, hitbox.height));
		shape.setPosition(sf::Vector2f(hitbox.left, hitbox.top));
		shape.setFillColor(color);
		window.draw(shape);
	}
}

void fr::Player::takeDamage(int damage) {
	health = std::clamp(health - damage, 0, max_health);
}

void fr::Player::takePermaDamage(int damage) {
	max_health = std::clamp(max_health - damage, 0, stats.max_health);
}

void fr::Player::takeHeadHit(fr::Punch punch) {
	if (state.guard_high == !punch.body && state.guard_low == punch.body) {
		takeDamage(punch.block_damage);
		// TODO Blockstun
	} else {
		takeDamage(punch.damage);
		takePermaDamage(punch.perma_damage);
		// TODO Hitstun
	}
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
