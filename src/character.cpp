#include "character.hpp"

#include <vector>

#include "SFML/Audio.hpp"
#include "animation.hpp"
#include "animations.hpp"
#include "dodge.hpp"
#include "punch.hpp"
#include "stats.hpp"

fr::Character::Character(sf::Sound jab, sf::Sound power) {
	stats = fr::Stats{
		20,								// max_health
		1,								// min_health
		1,								// health_regen
		1.f,							// regen_rate
		2,								// min_ko_damage
		92.f,							// velocity
		sf::Vector2f(32, 64),			// bounds
		sf::FloatRect(16, 3, 16, 16),	// head_hurtbox
		sf::FloatRect(12, 19, 16, 16),	// body_hurtbox
	};

	punches = std::vector<Punch>{
		// Jab
		fr::Punch(	fr::Control::jab, fr::Control::none,
					fr::Hit{2, 0, 0, 0.1f, 0.05f, true}, 0,
					0.125f, 0.2f, 0.25f, 0.3125f,
					sf::FloatRect(32, 8, 28, 8), sf::FloatRect(0, 8, 42, 8),
					fr::Animation(12, 5, false),
					jab),
		
		// Body jab		
		fr::Punch(	fr::Control::jab, fr::Control::body,
					fr::Hit{1, 0, 0, 0.05f, 0.05f, false}, 1,
					0.125f, 0.2f, 0.25f, 0.3125f,
					sf::FloatRect(32, 20, 28, 8), sf::FloatRect(0, 8, 40, 8),
					fr::Animation(13, 5, false), 
					jab),

		// Cross
		fr::Punch(	fr::Control::cross, fr::Control::none,
					fr::Hit{4, 0, 1, 0.25f, 0.125f, true}, 1,
					0.125f, 0.275f, 0.325f, 0.5f,
					sf::FloatRect(32, 8, 28, 8), sf::FloatRect(0, 8, 40, 8),
					fr::Animation(14, 7, false),
					power),
			
		// Body cross						
		fr::Punch(	fr::Control::cross, fr::Control::body,
					fr::Hit{1, 2, 1, 0.25f, 0.075f, false}, 1,
					0.125f, 0.275f, 0.325f, 0.5f,
					sf::FloatRect(32, 20, 28, 8), sf::FloatRect(0, 8, 36, 8),
					fr::Animation(15, 7, false),
					power),
		
		// Upper
		fr::Punch(	fr::Control::upper, fr::Control::none,
					fr::Hit{6, 0, 1, 0.25f, 0.1f, true}, 2,
					0.125f, 0.3125f, 0.35f, 0.55f, 
					sf::FloatRect(24, 8, 16, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation(16, 8, false),
					power),
		
		// Body upper						
		fr::Punch(	fr::Control::upper, fr::Control::body,
					fr::Hit{2, 4, 1, 0.25f, 0.05f, false}, 2,
					0.125f, 0.3125f, 0.35f, 0.55f, 
					sf::FloatRect(32, 20, 22, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation(17, 9, false),
					power),
			
		// Hook						
		fr::Punch(	fr::Control::hook, fr::Control::none,
					fr::Hit{6, 0, 3, 0.4f, 0.25f, true}, 2,
					0.1f, 0.325f, 0.425f, 0.75f,
					sf::FloatRect(32, 8, 12, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation(18, 9, false), 
					power),
		
		// Body hook						
		fr::Punch(	fr::Control::hook, fr::Control::body,
					fr::Hit{2, 4, 1, 0.3f, 0.15f, false}, 2,
					0.1f, 0.325f, 0.425f, 0.75f,
					sf::FloatRect(32, 20, 12, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation(19, 9, false), 
					power),
	};

	dodges = std::vector<Dodge>{
		fr::Dodge(1, 0.05, 0.275, 0.3756, 0, fr::Animation(11, 6, false)),	// Duck
		fr::Dodge(1, 0.05, 0.275, 0.3756, 42, fr::Animation(10, 6, false)),	// Slip
		fr::Dodge(1, 0.05, 0.275, 0.3756, 62, fr::Animation(9, 6, false)),	// Pull
	};

	animations = fr::Animations{
		fr::Animation(0, 1, false),	// idle
		fr::Animation(1, 1, false),	// idle_guard_head
		fr::Animation(2, 1, false),	// idle_guard_body
		fr::Animation(3, 8, true),		// walk
		fr::Animation(4, 8, true),		// walk_guard_head
		fr::Animation(5, 8, true),		// walk_guard_body
		fr::Animation(6, 1, false),	// hit_head
		fr::Animation(7, 1, false),	// hit_body
		fr::Animation(8, 1, false),	// ko
	};
}
