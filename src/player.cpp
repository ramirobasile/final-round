#include "player.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "direction.hpp"
#include "dodge.hpp"
#include "guard.hpp"
#include "health.hpp"
#include "input.hpp"
#include "input_manager.hpp"
#include "movement.hpp"
#include "physics.hpp"
#include "punch.hpp"
#include "sprite.hpp"
#include "stats.hpp"
#include "stun.hpp"
#include "utils.hpp"

fr::Player::Player(fr::Direction direction, fr::Device input_dev,
		std::vector<int> controls, fr::Stats stats, sf::Texture r_spritesheet,
		sf::Texture l_spritesheet, std::vector<fr::Animation> animations,
		int joystick)
		: direction(direction), stats(stats) {
	input_manager = InputManager(input_dev, controls, joystick);

	bounds = sf::FloatRect(0, 0, stats.bounds.x, stats.bounds.y);
	head_hurtbox = stats.head_hurtbox;
	body_hurtbox = stats.body_hurtbox;

	health = Health(stats.max_health, stats.min_health, stats.health_regen, 
			stats.regen_rate);

	punches = stats.punches;
	dodges = stats.dodges;

	sprite = Sprite(r_spritesheet, l_spritesheet, animations);
}

void fr::Player::update(float opponent_distance, float dt) {
	input_manager.update(dt);
	
	sprite.update(punch, prev_punch, dodge, prev_dodge, dt);
	
	if (stun_time >= 0)
		stun_time -= dt;
	else
		stun = Stun::none;
	
	if (getGuard() == Guard::none)
		health.regen(dt);
		
	if (!isReady())
		health.resetRegen();

	prev_punch = punch;
	punch.update(dt);
	
	// Take self damage when punch becomes uninterruptible
	if (prev_punch.canInterrupt() && !punch.canInterrupt())
		health.takeDamage(punch.getCost());

	// Feint when punch button release before it's uninterruptible
	if (punch.canInterrupt() 
			&& input_manager.inputted(punch.getControl(), Action::release)) {
		punch.end();
		health.resetRegen();
	}
	
	if (isReady())
		setNewPunch();

	prev_dodge = dodge;
	dodge.update(dt);
	
	// Take self damage when dodge starts
	if (prev_dodge.isDone() && !dodge.isDone())
		health.takeDamage(dodge.getCost());

	if (isReady())
		setNewDodge(opponent_distance);
}

void fr::Player::draw(sf::RenderWindow& window) {
	sprite.draw(window, getMovement(), getGuard(), punch, dodge, stun, 
			getBounds(), direction);
}

void fr::Player::takeHit(Hit hit) {
	if (getGuard() == Guard::head && hit.head || getGuard() == Guard::body) {
		health.takeDamage(hit.block_damage);
		
		stun_time = hit.block_stun;
		if (hit.head)
			stun = Stun::block_head;
		else
			stun = Stun::block_body;
	} else {
		if (health.getCurrent() <= 1 && hit.damage >= stats.min_ko_damage)
			ko = true;

		health.takeDamage(hit.damage);
		health.takePermaDamage(hit.perma_damage);
		
		stun_time = hit.hit_stun;
		if (hit.head)
			stun = Stun::head;
		else
			stun = Stun::body;

		if (punch.canInterrupt())
			punch.end();
	}
}

sf::Vector2f fr::Player::getVelocity() const {
	sf::Vector2f velocity;
	
	if (dodge.isDone() && (punch.isDone() || punch.canInterrupt())) {
		switch (getMovement()) {
			case fr::Movement::walk_b:
				velocity.x = -stats.velocity;
				break;

			case fr::Movement::walk_f:
				velocity.x = stats.velocity;
				break;

			case Movement::idle:
			case Movement::stun:
				break;
		}
	}
	
	return velocity;
}

sf::FloatRect fr::Player::getBounds() const {
	return sf::FloatRect(position.x, position.y, bounds.width, bounds.height);
}

sf::FloatRect fr::Player::getHeadHurtbox() const {
	if (dodge.isActive())
		return sf::FloatRect(0, 0, 0, 0);
		
	int left = getBounds().left;
	if (direction == Direction::right)
		left += head_hurtbox.left;
	else
		left += - head_hurtbox.left + getBounds().width - head_hurtbox.width;
	
	int top = getBounds().top + head_hurtbox.top;

	return sf::FloatRect(left, top, head_hurtbox.width, head_hurtbox.height);
}

sf::FloatRect fr::Player::getBodyHurtbox() const {
	int left = getBounds().left;
	if (direction == Direction::right)
		left += body_hurtbox.left;
	else
		left += - body_hurtbox.left + getBounds().width - body_hurtbox.width;

	int top = getBounds().top + body_hurtbox.top;

	return sf::FloatRect(left, top, body_hurtbox.width, body_hurtbox.height);
}

const fr::Health& fr::Player::getHealth() const {
	return health;
}

fr::Direction fr::Player::getDirection() const {
	return direction;
}

fr::Movement fr::Player::getMovement() const {
	Movement movement = Movement::idle;

	if (input_manager.inputted(Control::backwards))
		movement = Movement::walk_b;
	else if (input_manager.inputted(Control::forwards))
		movement = Movement::walk_f;
		
	if (stun_time > 0)
		movement = Movement::stun;

	return movement;
}

fr::Guard fr::Player::getGuard() const {
	Guard guard = Guard::none;

	if (input_manager.inputted(Control::up))
		guard = Guard::head;
	else if (input_manager.inputted(Control::down))
		guard = Guard::body;

	return guard;
}

fr::Stun fr::Player::getStun() const {
	return stun;
}

float fr::Player::getStunTime() const {
	return stun_time;
}

bool fr::Player::isReady() const {
	return getMovement() != Movement::stun && punch.isDone() && dodge.isDone();
}

void fr::Player::setNewPunch() {
	for (int i = 0; i < punches.size(); ++i) {
		if (input_manager.inputted(punches[i].getControl(), Action::press)
				&& input_manager.inputted(punches[i].getMod())) {
			punch = punches[i];
			punch.reset();
		}
	}
}

void fr::Player::setNewDodge(float opponent_distance) {
	if (input_manager.inputted(Control::dodge, Action::press)) {
		for (int i = 0; i < dodges.size(); ++i) {
			if (opponent_distance > dodges[i].getMinDistance())
				dodge = dodges[i];
		}
		
		dodge.reset();
	}
}
