#pragma once

#include <vector>
#include <string>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "punch.hpp"
#include "sprite.hpp"
#include "stats.hpp"

namespace fr {

class Player {
	public:
	int index;
	std::string alias;
	int direction;
	sf::IntRect bounds;
	int health;
	int max_health;
	State state;
	std::vector<Input> inputs;
	std::vector<Input> buffer;

	Player();
	Player(int index, std::string alias, int direction, Device input_dev,
			std::vector<int> controls, sf::Vector2f position, fr::Sprite sprite, 
			Stats stats);

	void update(float dt, int global_time, std::vector<sf::IntRect> geometry,
			fr::Player &opponent);
	void draw(sf::RenderWindow &window);
	void takeDamage(int damage);
	void takePermaDamage(int damage);
	void takeHit(fr::Punch &punch, bool head);
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::IntRect getHeadHurtbox() const;
	sf::IntRect getBodyHurtbox() const;
	void logState(int global_time);
	void logInputs(int global_time);
	void drawDebugGeometry(sf::RenderWindow &window);
	void drawDebugHurtboxes(sf::RenderWindow &window);
	void drawDebugHitboxes(sf::RenderWindow &window);

	private:
	Device input_dev;
	std::vector<int> controls;
	float buffer_ttl;
	sf::Vector2f velocity;
	sf::IntRect head_hurtbox;
	sf::IntRect body_hurtbox;
	fr::Sprite sprite;
	int animation;
	State last_state;
	Stats stats;
	std::vector<fr::Punch> punches;
	float tt_regen = 0;

	void updateVelocity(sf::Vector2f &velocity, State state, State last_state,
			Stats stats);
	std::vector<Collision> getCollisions(sf::IntRect rect1,
			std::vector<sf::IntRect> rects);
};

} // namespace fr
