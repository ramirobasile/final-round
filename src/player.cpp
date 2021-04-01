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

// Empty constructor
fr::Player::Player() {
}

fr::Player::Player(int index, int direction, fr::Device input_dev,
		std::vector<int> controls, sf::Vector2f position, 
		sf::Texture spritesheet, std::vector<fr::Animation> animations,
		fr::Stats stats)
		: index(index), direction(direction), input_dev(input_dev), 
		controls(controls), animations(animations), stats(stats) {
	bounds = sf::IntRect(position.x, position.y, 48, 48);
	head_hurtbox = sf::IntRect(32, 0, 16, 16);
	body_hurtbox = sf::IntRect(24, 16, 16, 16);

	sprite.setTexture(spritesheet);
	sprite.setTextureRect(bounds);

	punches = default_punches;

	max_health = stats.max_health;
	health = max_health;
}

void fr::Player::update(std::vector<sf::IntRect> geometry, fr::Player opponent) {
	// INPUT
	updateBuffer(buffer, buffer_ttl, inputs);
	if (input_dev == Device::keyboard)
		updateInputs(inputs, buffer, buffer_ttl, controls);
	else if (input_dev == Device::joystick)
		updateInputs(index, inputs, buffer, buffer_ttl, controls);

	// STATE
	last_state = state;
	state.update(inputs, buffer, punches);

	// Regenerate
	regen_timer += dt;
	if (!state.punching() && !state.guarding() && regen_timer > stats.regen_rate) {
		takeDamage(-stats.health_regen);
		regen_timer = 0;
	}

	// Interrupt punches
	sf::IntRect clearbox = state.punch.getClearbox(bounds, direction);
	bool obstructed = clearbox.intersects(opponent.headHurtbox())
			|| clearbox.intersects(opponent.bodyHurtbox());
	if (state.punch.interruptible() && obstructed)
		state.punch.interrupt();

	bool was_int = last_state.punch.interruptible();
	if (was_int && !state.punch.interruptible())
		takeDamage(state.punch.self_damage);

	// Hit
	sf::IntRect hitbox = state.punch.getHitbox(bounds, direction);
	if (state.punch.active() && hitbox.intersects(opponent.headHurtbox())) {
		opponent.takeHeadHit(state.punch);
		state.punch.interrupt();
	}

	if (state.punch.active() && hitbox.intersects(opponent.bodyHurtbox())) {
		opponent.takeBodyHit(state.punch);
		state.punch.interrupt();
	}

	// PHYSICS
	updateVelocity(velocity, state, last_state, stats);
	updatePosition(bounds, velocity);
	resolveCollision(bounds, opponent.bounds);
	for (int i = 0; i < geometry.size(); ++i)
		resolveCollision(bounds, geometry[i]);

	// DEBUG
	std::string pre = "[" + getGlobalTime() + "] [P" +  std::to_string(index) + "] ";

	if (config.getBool("debug", "log_state", false)) {
		if (state.movement != last_state.movement)
			std::cout << pre + "Movement: " + std::to_string((int)state.movement) << std::endl;

		if (state.punching())
			std::cout << pre + "Punch: " + std::to_string(state.punch.progress) << std::endl;
	}

	if (config.getBool("debug", "log_inputs", false) && !inputs.empty()) {
		std::cout << pre + "Inputs:" << std::endl;
		for (int i = 0; i < inputs.size(); ++i)
			std::cout << "* " + (std::string)inputs[i] << std::endl;
	}
}

void fr::Player::draw(sf::RenderWindow &window) {
	//Animation animaton = animations[animation];

	//sprite.setTextureRect(sf::IntRect(animaton.frame * animaton.width,
	//		animation * animaton.height, animaton.width, animaton.height));

	// DEBUG
	if (config.getBool("debug", "draw_geometry", false)) {
		sf::RectangleShape shape(size());
		shape.setPosition(position());
		shape.setFillColor(sf::Color::Cyan);
		window.draw(shape);
	}

	if (config.getBool("debug", "draw_hurtboxes", false)) {
		sf::RectangleShape head(sf::Vector2f(headHurtbox().width, headHurtbox().height));
		head.setPosition(headHurtbox().left, headHurtbox().top);
		head.setFillColor(sf::Color::Yellow);
		window.draw(head);

		sf::RectangleShape body(sf::Vector2f(bodyHurtbox().width, bodyHurtbox().height));
		body.setPosition(bodyHurtbox().left, bodyHurtbox().top);
		body.setFillColor(sf::Color::Yellow);
		window.draw(body);
	}

	if (config.getBool("debug", "draw_hitboxes", false) && state.punching()) {
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

void fr::Player::takeDamage(int damage) {
	health = std::clamp(health - damage, 0, max_health);
}

void fr::Player::takePermaDamage(int damage) {
	max_health = std::clamp(max_health - damage, 0, stats.max_health);
}

void fr::Player::takeHeadHit(fr::Punch punch) {
	if (state.guard_high) {
		takeDamage(punch.block_damage);
		// TODO Blockstun
	} else {
		takeDamage(punch.damage);
		takePermaDamage(punch.perma_damage);
		// TODO Hitstun
	}
}

void fr::Player::takeBodyHit(fr::Punch punch) {
	if (state.guard_low) {
		takeDamage(punch.block_damage);
		// TODO Blockstun
	} else {
		takeDamage(punch.damage);
		takePermaDamage(punch.perma_damage);
		// TODO Hitstun
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
