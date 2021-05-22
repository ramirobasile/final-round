#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "animation.hpp"
#include "dodge.hpp"
#include "input.hpp"

namespace fr {

struct Stats {
	int max_health = 20;
	int min_health = 1;
	int health_regen = 1;
	float regen_rate = 1;
	int min_ko_damage = 2;

	float velocity = 92;
	
	sf::Vector2f bounds = sf::Vector2f(32, 64);
	sf::FloatRect head_hurtbox = sf::FloatRect(16, 3, 16, 16);
	sf::FloatRect body_hurtbox = sf::FloatRect(12, 19, 16, 16);
	
	Punch jab = Punch(			Control::jab, Control::none,
								Hit{2, 0, 0, 0.1f, 0.05f, true}, 1,
								0.125f, 0.2f, 0.25f, 0.3125f,
								sf::FloatRect(32, 8, 28, 8), sf::FloatRect(0, 8, 42, 8),
								Animation::Name::jab);
								
	Punch body_jab = Punch(		Control::jab, Control::body,
								Hit{1, 0, 0, 0.05f, 0.05f, false}, 1,
								0.125f, 0.2f, 0.25f, 0.3125f,
								sf::FloatRect(32, 20, 28, 8), sf::FloatRect(0, 8, 40, 8),
								Animation::Name::jab_body);

	Punch cross = Punch(		Control::cross, Control::none,
								Hit{4, 0, 1, 0.25f, 0.125f, true}, 1,
								0.125f, 0.275f, 0.325f, 0.5f,
								sf::FloatRect(32, 8, 28, 8), sf::FloatRect(0, 8, 40, 8),
								Animation::Name::cross);
								
	Punch body_cross = Punch(	Control::cross, Control::body,
								Hit{1, 2, 1, 0.25f, 0.075f, false}, 1,
								0.125f, 0.275f, 0.325f, 0.5f,
								sf::FloatRect(32, 20, 28, 8), sf::FloatRect(0, 8, 36, 8),
								Animation::Name::cross_body);

	Punch upper = Punch(		Control::upper, Control::none,
								Hit{6, 0, 1, 0.25f, 0.1f, true}, 1,
								0.125f, 0.3125f, 0.35f, 0.55f, 
								sf::FloatRect(24, 8, 16, 8), sf::FloatRect(0, 0, 0, 0),
								Animation::Name::upper);
								
	Punch body_upper = Punch(	Control::upper, Control::body,
								Hit{2, 4, 1, 0.25f, 0.05f, false}, 1,
								0.125f, 0.3125f, 0.35f, 0.55f, 
								sf::FloatRect(24, 20, 16, 8), sf::FloatRect(0, 0, 0, 0),
								Animation::Name::upper_body);
								
	Punch hook = Punch(			Control::hook, Control::none,
								Hit{6, 0, 3, 0.4f, 0.25f, true}, 2,
								0.1f, 0.325f, 0.425f, 0.75f,
								sf::FloatRect(32, 8, 12, 8), sf::FloatRect(0, 0, 0, 0),
								Animation::Name::hook);
								
	Punch body_hook = Punch(	Control::hook, Control::body,
								Hit{2, 4, 1, 0.3f, 0.15f, false}, 2,
								0.1f, 0.325f, 0.425f, 0.75f,
								sf::FloatRect(32, 20, 12, 8), sf::FloatRect(0, 0, 0, 0),
								Animation::Name::hook_body);

	std::vector<Punch> punches = {jab, body_jab, cross, body_cross, upper, 
			body_upper, hook, body_hook};
	
	Dodge duck = Dodge(1, 0.05, 0.275, 0.3756, 0, Animation::Name::duck);
	Dodge slip = Dodge(1, 0.05, 0.275, 0.3756, 42, Animation::Name::slip);
	Dodge pull = Dodge(1, 0.05, 0.275, 0.3756, 62, Animation::Name::pull);
	
	std::vector<Dodge> dodges = {duck, slip, pull};
};

} // namespace fr
