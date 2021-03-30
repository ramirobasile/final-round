#pragma once

#include <vector>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "punch.hpp"
#include "animation.hpp"
#include "stats.hpp"

namespace fr {

class Player {
	public:
	int index;
	int direction;
	sf::IntRect bounds;

	Player(int index, int direction, Device input_dev,
			std::vector<int> controls, sf::Vector2f position,
			sf::Texture spritesheet, std::vector<Animation> animations,
			Stats stats);

	void update(std::vector<sf::IntRect> geometry, fr::Player opponent);
	void draw(sf::RenderWindow &window);
	void takeDamage(int damage);
	void takePermaDamage(int damage);
	void takeHeadHit(fr::Punch punch);
	void takeBodyHit(fr::Punch punch);
	sf::Vector2f position() const;
	sf::Vector2f size() const;
	sf::IntRect headHurtbox() const;
	sf::IntRect bodyHurtbox() const;

	private:
	Device input_dev;
	std::vector<int> controls;
	std::vector<Input> inputs;
	std::vector<Input> buffer;
	float buffer_ttl;
	sf::Vector2f velocity;
	sf::IntRect head_hurtbox;
	sf::IntRect body_hurtbox;
	int health;
	int max_health;
	sf::Sprite sprite;
	std::vector<Animation> animations;
	int animation;
	State state;
	State last_state;
	Stats stats;
	std::vector<fr::Punch> punches;

	void updateVelocity(sf::Vector2f &velocity, State state, State last_state,
			Stats stats);
	std::vector<Collision> getCollisions(sf::IntRect rect1,
			std::vector<sf::IntRect> rects);
};

} // namespace fr
