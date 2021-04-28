#pragma once

#include <vector>
#include <string>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
#include "punch.hpp"
#include "dodge.hpp"
#include "sprite.hpp"
#include "stats.hpp"
#include "direction.hpp"

namespace fr {

class Player {
	public:
	int index;
	std::string alias;
	Direction direction;
	sf::FloatRect bounds;
	int health;
	int max_health;
	State state;
	std::vector<Input> inputs;
	std::vector<Input> buffer;
	bool dead = false;

	Player();
	Player(int index, std::string alias, Direction direction, Device input_dev,
			std::vector<int> controls, sf::Vector2f position, fr::Sprite sprite, 
			Stats stats);

	void update(float dt, std::vector<sf::FloatRect> geometry, 
			fr::Player &opponent);
	void draw(sf::RenderWindow &window);
	void takeDamage(int damage);
	void takePermaDamage(int damage);
	void takeHit(fr::Punch &punch, bool head);
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getHeadHurtbox() const;
	sf::FloatRect getBodyHurtbox() const;
	void drawDebugGeometry(sf::RenderWindow &window);
	void drawDebugHurtboxes(sf::RenderWindow &window);
	void drawDebugHitboxes(sf::RenderWindow &window);

	private:
	Device input_dev;
	std::vector<int> controls;
	float buffer_ttl;
	sf::Vector2f velocity;
	sf::FloatRect head_hurtbox;
	sf::FloatRect body_hurtbox;
	fr::Sprite sprite;
	State last_state;
	Stats stats;
	float tt_regen = 0;

	void updateVelocity(sf::Vector2f &velocity, State state, State last_state,
			Stats stats);
	std::vector<Collision> getCollisions(sf::FloatRect rect1,
			std::vector<sf::FloatRect> rects);
};

} // namespace fr
