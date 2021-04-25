#pragma once

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "dodge.hpp"
#include "input.hpp"
#include "animations.hpp"

namespace fr {

struct Stats {
	int max_health = 20;
	float health_regen = 1;
	float regen_rate = 1;

	float walk_speed = 92;
	
	sf::Vector2f bounds = sf::Vector2f(32, 64);
	sf::FloatRect head_hurtbox = sf::FloatRect(16, 3, 16, 16);
	sf::FloatRect body_hurtbox = sf::FloatRect(12, 19, 16, 16);
	
	std::vector<Dodge> dodges = {
		Dodge(), // None
		
		// Pull
		Dodge(Controls::left, Actions::double_press,
				0,
				0, 0.25f, 0.3f, 
				sf::Vector2f(-8, 0), Animations::pull),
		
		// Slip
		Dodge(Controls::up, Actions::double_press, 
				0,
				0, 0.225f, 0.225f, 
				sf::Vector2f(999, 0), Animations::slip),
		
		// Duck
		Dodge(Controls::down, Actions::double_press, 
				1,
				0.025f, 0.275f, 0.3f, 
				sf::Vector2f(2, 8), Animations::duck),
	};
	
	std::vector<Punch> punches = {
		Punch(), // None
		
		// Body jab
		Punch(Controls::a, Actions::release, Controls::down, 0, PRESS_END,
				1, 0, 0, 1, 
				0.15f, 0.2f, 0.25f, 0.3f, 
				true, sf::FloatRect(32, 20, 28, 8), Animations::jab_body),
		// Head jab
		Punch(Controls::a, Actions::release, Controls::none, 0, PRESS_END,
				2, 0, 0, 1, 
				0.15f, 0.2f, 0.25f, 0.3f,
				true, sf::FloatRect(32, 8, 28, 8), Animations::jab_head),

		// Body cross
		Punch(Controls::a, Actions::hold, Controls::down, PRESS_END, PRESS_END + 0.1f,
				2, 1, 1, 1, 
				0.15f, 0.2f, 0.25f, 0.5f, 
				true, sf::FloatRect(32, 20, 28, 8), Animations::cross_body),

		// Head cross
		Punch(Controls::a, Actions::hold, Controls::none, PRESS_END, PRESS_END + 0.1f,
				4, 0, 1, 1, 
				0.15f, 0.275f, 0.325f, 0.5f,
				true, sf::FloatRect(32, 8, 28, 8), Animations::cross_head),

		// Body upper
		Punch(Controls::b, Actions::release, Controls::down, 0, PRESS_END,
				3, 2, 1, 1, 
				0.15f, 0.3125f, 0.35f, 0.625f, 
				false, sf::FloatRect(26, 20, 16, 12), Animations::upper_body),

		// Head upper
		Punch(Controls::b, Actions::release, Controls::none, 0, PRESS_END,
				4, 0, 1, 1, 
				0.15f, 0.3125f, 0.35f, 0.625f, 
				true, sf::FloatRect(26, 4, 16, 12), Animations::upper_head),

		// Body hook
		Punch(Controls::b, Actions::hold, Controls::down, PRESS_END, PRESS_END + 0.1f,
				2, 2, 2, 2, 
				0.1f, 0.325f, 0.425f, 0.75f, 
				false, sf::FloatRect(30, 20, 16, 12), Animations::hook_body),

		// Head hook
		Punch(Controls::b, Actions::hold, Controls::none, PRESS_END, PRESS_END + 0.1f,
				6, 0, 2, 2, 
				0.1f, 0.325f, 0.425f, 0.75f,
				false, sf::FloatRect(30, 4, 16, 12), Animations::hook_head),
	};
};

} // namespace fr
