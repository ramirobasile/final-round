#pragma once

#include <string>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
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

namespace fr {

class Player {
	public:
	Player() {}; // Empty
	Player(Direction direction, Device input_dev, std::vector<int> controls,
			Stats stats, sf::Texture r_spritesheet, sf::Texture l_spritesheet,
			std::vector<Animation> base_animations, int joystick = -1);

	void update(float opponent_distance, float dt);
	void draw(sf::RenderWindow &window);
	void takeHit(fr::Hit hit);
	sf::Vector2f getVelocity() const;
	sf::FloatRect getBounds() const;
	sf::FloatRect getHeadHurtbox() const;
	sf::FloatRect getBodyHurtbox() const;
	int getHealth() const;
	int getMaxHealth() const;
	Direction getDirection() const;
	Movement getMovement() const;
	Guard getGuard() const;
	bool isReady() const;

	sf::Vector2f position;
	Punch punch;

	private:
	void setNewPunch();
	void setNewDodge(float opponent_distance);
	
	Direction direction;
	Stats stats;
	sf::FloatRect bounds;
	InputManager input_manager;
	sf::FloatRect head_hurtbox;
	sf::FloatRect body_hurtbox;
	Health health;
	Punch prev_punch;
	std::vector<Punch> punches;
	Dodge dodge;
	Dodge prev_dodge;
	std::vector<Dodge> dodges;
	fr::Sprite sprite;
	bool ko = false;
};

} // namespace fr
