#pragma once

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

namespace fr {

class Player {
	public:
	Player() {}; // Empty
	Player(Direction direction, Device input_dev, std::vector<int> controls,
			Stats stats, std::vector<Punch> punches, Dodge dodge,
			sf::Texture r_spritesheet, sf::Texture l_spritesheet, 
			Animations animations, Sounds sounds, int joystick = -1);

	void update(float dt);
	void draw(sf::RenderWindow &window);
	void takeHit(fr::Hit hit);
	sf::Vector2f getVelocity() const;
	sf::FloatRect getBounds() const;
	const Health& getHealth() const;
	Direction getDirection() const;
	Movement getMovement() const;
	Guard getGuard() const;
	bool isKO() const;
	Stun getStun() const;
	float getStunTime() const;
	bool isReady() const;

	sf::Vector2f position;
	Punch punch;

	private:
	Direction direction = Direction::left;
	Stats stats;
	std::vector<Punch> punches;
	Sounds sounds;
	InputManager input_manager;
	sf::FloatRect bounds;
	Health health;
	Punch prev_punch;
	Dodge dodge;
	Dodge prev_dodge;
	fr::Sprite sprite;
	fr::Stun stun = Stun::none;
	float stun_time = 0;
	bool ko = false;
};

} // namespace fr
