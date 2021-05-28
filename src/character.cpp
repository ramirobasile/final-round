#include "character.hpp"

#include <vector>

#include "animation.hpp"
#include "dodge.hpp"
#include "punch.hpp"
#include "stats.hpp"

//////////////////////////////////////////////////////////////////////
// Base character
//////////////////////////////////////////////////////////////////////

fr::Character base {
	fr::Stats {
		20,								// max_health
		1,								// min_health
		1,								// health_regen
		1.f,							// regen_rate
		2,								// min_ko_damage
		92.f,							// velocity
		sf::Vector2f(32, 64),			// bounds
		sf::FloatRect(16, 3, 16, 16),	// head_hurtbox
		sf::FloatRect(12, 19, 16, 16),	// body_hurtbox
	},

	{
		// Jab
		fr::Punch(	fr::Control::jab, fr::Control::none,
					fr::Hit{2, 0, 0, 0.1f, 0.05f, true}, 1,
					0.125f, 0.2f, 0.25f, 0.3125f,
					sf::FloatRect(32, 8, 28, 8), sf::FloatRect(0, 8, 42, 8),
					fr::Animation::Name::jab),
		
		// Body jab		
		fr::Punch(	fr::Control::jab, fr::Control::body,
					fr::Hit{1, 0, 0, 0.05f, 0.05f, false}, 1,
					0.125f, 0.2f, 0.25f, 0.3125f,
					sf::FloatRect(32, 20, 28, 8), sf::FloatRect(0, 8, 40, 8),
					fr::Animation::Name::jab_body),

		// Cross
		fr::Punch(	fr::Control::cross, fr::Control::none,
					fr::Hit{4, 0, 1, 0.25f, 0.125f, true}, 1,
					0.125f, 0.275f, 0.325f, 0.5f,
					sf::FloatRect(32, 8, 28, 8), sf::FloatRect(0, 8, 40, 8),
					fr::Animation::Name::cross),
			
		// Body cross						
		fr::Punch(	fr::Control::cross, fr::Control::body,
					fr::Hit{1, 2, 1, 0.25f, 0.075f, false}, 1,
					0.125f, 0.275f, 0.325f, 0.5f,
					sf::FloatRect(32, 20, 28, 8), sf::FloatRect(0, 8, 36, 8),
					fr::Animation::Name::cross_body),
		
		// Upper
		fr::Punch(	fr::Control::upper, fr::Control::none,
					fr::Hit{6, 0, 1, 0.25f, 0.1f, true}, 2,
					0.125f, 0.3125f, 0.35f, 0.55f, 
					sf::FloatRect(24, 8, 16, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation::Name::upper),
		
		// Body upper						
		fr::Punch(	fr::Control::upper, fr::Control::body,
					fr::Hit{2, 4, 1, 0.25f, 0.05f, false}, 2,
					0.125f, 0.3125f, 0.35f, 0.55f, 
					sf::FloatRect(32, 20, 22, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation::Name::upper_body),
			
		// Hook						
		fr::Punch(	fr::Control::hook, fr::Control::none,
					fr::Hit{6, 0, 3, 0.4f, 0.25f, true}, 2,
					0.1f, 0.325f, 0.425f, 0.75f,
					sf::FloatRect(32, 8, 12, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation::Name::hook),
		
		// Body hook						
		fr::Punch(	fr::Control::hook, fr::Control::body,
					fr::Hit{2, 4, 1, 0.3f, 0.15f, false}, 2,
					0.1f, 0.325f, 0.425f, 0.75f,
					sf::FloatRect(32, 20, 12, 8), sf::FloatRect(0, 0, 0, 0),
					fr::Animation::Name::hook_body),
	},

	{
		fr::Dodge(1, 0.05, 0.275, 0.3756, 0, fr::Animation::Name::duck),	// Duck
		fr::Dodge(1, 0.05, 0.275, 0.3756, 42, fr::Animation::Name::slip),	// Slip
		fr::Dodge(1, 0.05, 0.275, 0.3756, 62, fr::Animation::Name::pull),	// Pull
	},

	{
		fr::Animation(1, false),	// idle
		fr::Animation(1, false),	// idle_guard_head
		fr::Animation(1, false),	// idle_guard_body
		fr::Animation(8, true),		// walk
		fr::Animation(8, true),		// walk_guard_head
		fr::Animation(8, true),		// walk_guard_body
		fr::Animation(6, false),	// pull
		fr::Animation(6, false),	// slip
		fr::Animation(6, false),	// duck
		fr::Animation(1, false),	// hit_head
		fr::Animation(1, false),	// hit_body
		fr::Animation(1, false),	// ko
		fr::Animation(5, false),	// jab
		fr::Animation(5, false),	// jab_body
		fr::Animation(7, false),	// cross
		fr::Animation(7, false),	// cross_body
		fr::Animation(8, false),	// upper
		fr::Animation(9, false),	// upper_body
		fr::Animation(9, false),	// hook
		fr::Animation(9, false),	// hook_body
	},
};

std::vector<fr::Character> fr::characters = std::vector<fr::Character>{base};
