#include "player.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "animations.hpp"
#include "direction.hpp"
#include "dodge.hpp"
#include "guard.hpp"
#include "health.hpp"
#include "input.hpp"
#include "input_manager.hpp"
#include "movement.hpp"
#include "physics.hpp"
#include "punch.hpp"
#include "sounds.hpp"
#include "sprite.hpp"
#include "stats.hpp"
#include "stun.hpp"
#include "utils.hpp"

fr::Player::Player(fr::Direction direction, fr::Device input_dev,
		std::vector<int> controls, fr::Stats stats, 
		std::vector<fr::Punch> punches, fr::Dodge dodge, 
		sf::Texture r_spritesheet, sf::Texture l_spritesheet, 
		Animations animations, Sounds sounds, int joystick)
		: direction(direction), stats(stats), punches(punches), dodge(dodge),
		sounds(sounds) {
	input_manager = InputManager(input_dev, controls, joystick);

	bounds = stats.bounds;

	health = Health(stats.max_health, stats.min_health, stats.health_regen, 
			stats.regen_rate);

	sprite = Sprite(r_spritesheet, l_spritesheet, animations);
}

void fr::Player::update(float dt) {
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
	
	// Attempt to throw punch
	if (isReady()) {
		for (int i = 0; i < punches.size(); ++i) {
			if (input_manager.inputted(punches[i].getControl(), Action::press)
					&& input_manager.inputted(punches[i].getMod())) {
				punch = punches[i];
				punch.reset();
			}
		}
	}
		
	// Play punch sound when punch starts
	if (prev_punch.isDone() && !punch.isDone()) {
		prev_punch.sound.stop();
		punch.sound.play();
	}

	// Take self damage when punch becomes uninterruptible
	if (prev_punch.canInterrupt() && !punch.canInterrupt())
		health.takeDamage(punch.getCost());

	// Feint when punch button release before it's uninterruptible
	if (punch.canInterrupt() 
			&& input_manager.inputted(punch.getControl(), Action::release)) {
		punch.end();
		punch.sound.stop();
		health.resetRegen();
	}

	prev_dodge = dodge;
	dodge.update(dt);

	if (isReady() && input_manager.inputted(Control::dodge, Action::press))
		dodge.reset();
		
	// Take self damage when dodge starts and play sound
	if (prev_dodge.isDone() && !dodge.isDone()) {
		health.takeDamage(dodge.getCost());
		sounds.dodge.stop();
		sounds.dodge.play();
	}
	
	// Play step sound when moving
	if (isReady() && (getMovement() == Movement::walk_b 
			|| getMovement() == Movement::walk_f)) {
		if (sounds.step.getStatus() != sf::SoundSource::Status::Playing)
			sounds.step.play();
	} else
		sounds.step.stop();
}

void fr::Player::draw(sf::RenderWindow& window) {
	sprite.draw(window, getMovement(), getGuard(), punch, dodge, stun, 
			getBounds(), direction);
}

void fr::Player::takeHit(Hit hit) {
	if ((getGuard() == Guard::head && hit.head) 
		|| (getGuard() == Guard::body && !hit.head)) {
		health.takeDamage(hit.block_damage);
		
		stun_time = hit.block_stun;
		if (hit.head)
			stun = Stun::block_head;
		else
			stun = Stun::block_body;
			
		sounds.block.play();
	} else {
		if (health.getCurrent() <= health.getMin() && hit.can_ko)
			ko = true;

		health.takeDamage(hit.damage);
		health.takePermaDamage(hit.perma_damage);
		health.resetRegen();
		
		stun_time = hit.hit_stun;
		if (hit.head)
			stun = Stun::head;
		else
			stun = Stun::body;

		if (punch.canInterrupt())
			punch.end();
			
		sounds.hit.play();
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

bool fr::Player::isKO() const {
	return ko;
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
