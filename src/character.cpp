#include "character.hpp"

#include <vector>

#include "SFML/Audio.hpp"
#include "animation.hpp"
#include "animations.hpp"
#include "dodge.hpp"
#include "punch.hpp"
#include "stats.hpp"

fr::Character::Character(sf::Sound jab_sound, sf::Sound power_sound) {
	stats = fr::Stats{
		20,								// max_health
		1,								// min_health
		1,								// health_regen
		1.f,							// regen_rate
		92.f,							// velocity
		sf::FloatRect(0, 0, 32, 64),	// bounds
	};

	fr::Punch jab = fr::Punch(	fr::Control::jab,
								fr::Hit{1, 0, 0, 0.1f, 0.05f, true, false}, 0,
								60, 42,
								0.125f, 0.2f, 0.25f, 0.3125f,
								fr::Animation(10, 5, false), jab_sound);
									
	fr::Punch body_jab = fr::Punch(	jab,
									fr::Hit{1, 0, 0, 0.05f, 0.05f, false, false},
									56, 38, 
									fr::Animation(11, 5, false));
					
	fr::Punch cross = fr::Punch(fr::Control::cross,
								fr::Hit{4, 0, 1, 0.25f, 0.125f, true, true}, 1,
								60, 38,
								0.125f, 0.275f, 0.325f, 0.5f,
								fr::Animation(12, 7, false), power_sound);
								
	fr::Punch body_cross = fr::Punch(	cross,	
										fr::Hit{1, 2, 1, 0.25f, 0.075f, false, false},
										56, 36, 
										fr::Animation(13, 7, false));
				
	fr::Punch upper = fr::Punch(fr::Control::upper,
								fr::Hit{6, 0, 1, 0.25f, 0.1f, true, true}, 2,
								44, 0,
								0.125f, 0.3125f, 0.35f, 0.55f,
								fr::Animation(14, 8, false), power_sound);
								
	fr::Punch body_upper = fr::Punch(	upper,	
										fr::Hit{2, 4, 1, 0.25f, 0.05f, false, true},
										50, 0, 
										fr::Animation(15, 8, false));
				
	fr::Punch hook = fr::Punch(	fr::Control::hook,
								fr::Hit{6, 0, 3, 0.4f, 0.25f, true, true}, 2,
								50, 0,
								0.1f, 0.325f, 0.425f, 0.75f,
								fr::Animation(16, 9, false), power_sound);
								
	fr::Punch body_hook = fr::Punch(hook,	
									fr::Hit{2, 4, 1, 0.3f, 0.15f, false, true},
									50, 0, 
									fr::Animation(17, 9, false));
					
	punches = std::vector<Punch>{jab, body_jab, cross, body_cross, upper, 
			body_upper, hook, body_hook};

	dodge = fr::Dodge(1, 0.05, 0.275, 0.3756, fr::Animation(9, 6, false));

	animations = fr::Animations{
		fr::Animation(0, 1, false),	// idle
		fr::Animation(1, 1, false),	// idle_guard_head
		fr::Animation(2, 1, false),	// idle_guard_body
		fr::Animation(3, 8, true),	// walk
		fr::Animation(4, 8, true),	// walk_guard_head
		fr::Animation(5, 8, true),	// walk_guard_body
		fr::Animation(6, 1, false),	// hit_head
		fr::Animation(7, 1, false),	// hit_body
		fr::Animation(8, 1, false),	// ko
	};
}
