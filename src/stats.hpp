#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "animations.hpp"
#include "dodge.hpp"
#include "input.hpp"

namespace fr {

struct Stats {
	int max_health = 20;
	float health_regen = 1;
	float regen_rate = 1;

	float walk_speed = 92;
	
	sf::Vector2f bounds = sf::Vector2f(32, 64);
	sf::FloatRect head_hurtbox = sf::FloatRect(16, 3, 16, 16);
	sf::FloatRect body_hurtbox = sf::FloatRect(12, 19, 16, 16);
	
	Dodge dodge = Dodge(1, 0.05, 0.275, 0.3);
	
	std::vector<Punch> punches = {
		Punch(), // None
		
		// Body jab
		Punch(Control::a, Action::release, Control::down, 0, PRESS_END,
				1, 0, 0, 1, 
				0.15f, 0.2f, 0.25f, 0.3f, 
				true, sf::FloatRect(32, 20, 28, 8), Animations::jab_body),
		// Head jab
		Punch(Control::a, Action::release, Control::none, 0, PRESS_END,
				2, 0, 0, 1, 
				0.15f, 0.2f, 0.25f, 0.3f,
				true, sf::FloatRect(32, 8, 28, 8), Animations::jab_head),

		// Body cross
		Punch(Control::a, Action::hold, Control::down, PRESS_END, PRESS_END + 0.1f,
				2, 1, 1, 1, 
				0.15f, 0.2f, 0.25f, 0.5f, 
				true, sf::FloatRect(32, 20, 28, 8), Animations::cross_body),

		// Head cross
		Punch(Control::a, Action::hold, Control::none, PRESS_END, PRESS_END + 0.1f,
				4, 0, 1, 1, 
				0.15f, 0.275f, 0.325f, 0.5f,
				true, sf::FloatRect(32, 8, 28, 8), Animations::cross_head),

		// Body upper
		Punch(Control::b, Action::release, Control::down, 0, PRESS_END,
				3, 2, 1, 1, 
				0.15f, 0.3125f, 0.35f, 0.625f, 
				false, sf::FloatRect(26, 20, 16, 12), Animations::upper_body),

		// Head upper
		Punch(Control::b, Action::release, Control::none, 0, PRESS_END,
				4, 0, 1, 1, 
				0.15f, 0.3125f, 0.35f, 0.55f, 
				true, sf::FloatRect(26, 4, 16, 12), Animations::upper_head),

		// Body hook
		Punch(Control::b, Action::hold, Control::down, PRESS_END, PRESS_END + 0.1f,
				2, 2, 2, 2, 
				0.15f, 0.3125f, 0.35f, 0.55f,
				false, sf::FloatRect(30, 20, 16, 12), Animations::hook_body),

		// Head hook
		Punch(Control::b, Action::hold, Control::none, PRESS_END, PRESS_END + 0.1f,
				6, 0, 2, 2, 
				0.1f, 0.325f, 0.425f, 0.75f,
				false, sf::FloatRect(30, 4, 16, 12), Animations::hook_head),
	};
};

} // namespace fr
