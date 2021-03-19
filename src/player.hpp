#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "utils.hpp"
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
		std::vector<sf::IntRect> hurtboxes;
		float body_health;
		float head_health;
		float guard_health;

		Player(int index, int direction, Device input_dev, 
				std::vector<int> controls, sf::Vector2f position,
				sf::Texture spritesheet, std::vector<Animation> animations, 
				Stats stats);

		void update(std::vector<sf::IntRect> geometry);
		void draw(sf::RenderWindow &window);
		sf::Vector2f position();
		sf::Vector2f size();

		private:
		Device input_dev;
		std::vector<int> controls;
		std::vector<Input> inputs;
		std::vector<Input> buffer;
		float buffer_ttl;
		sf::Vector2f velocity;
		sf::Sprite sprite;
		std::vector<Animation> animations;
		int animation;
		State state;
		State last_state;
		Stats stats;

		void updateVelocity(sf::Vector2f &velocity, State state,
				State last_state, Stats stats);
		std::vector<Collision> getCollisions(sf::IntRect rect1, 
				std::vector<sf::IntRect> rects);
	};
}
