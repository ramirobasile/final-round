#pragma once

#include <vector>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "state.hpp"
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
				std::vector<Punch> punches, Stats stats);

		void update(std::vector<sf::IntRect> geometry);
		void draw(sf::RenderWindow &window);
		sf::Vector2f getPosition();
		sf::Vector2f getSize();
		sf::IntRect getHeadHurtbox();
		sf::IntRect getBodyHurtbox();

		private:
		Device input_dev;
		std::vector<int> controls;
		std::vector<Input> inputs;
		std::vector<Input> buffer;
		float buffer_ttl;
		sf::Vector2f velocity;
		sf::IntRect head_hurtbox;
		sf::IntRect body_hurtbox;
		float body_health;
		float head_health;
		float guard_health;
		sf::Sprite sprite;
		std::vector<Animation> animations;
		int animation;
		std::vector<Punch> punches;
		State state;
		State last_state;
		Stats stats;

		void updateVelocity(sf::Vector2f &velocity, State state,
				State last_state, Stats stats);
		std::vector<Collision> getCollisions(sf::IntRect rect1, 
				std::vector<sf::IntRect> rects);
	};
}
