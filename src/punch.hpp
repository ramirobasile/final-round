#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "input.hpp"

namespace fr {
	class Punch {
		public:
		Punch(Control control, sf::IntRect hitbox, bool is_lead_handed,
				bool is_body, int damange, int body_hp_cost, float hitbox_begin,
				float hitbox_end, float recovery_end);
		
		const Control BODY_CONTROL = Control::down;
		const float BODY_Y_OFFSET = 10;
		
		Control control;
		sf::IntRect hitbox;
		bool is_lead_handed;
		bool is_body;
		int damage;
		int body_hp_cost;
		float hitbox_begin;
		float hitbox_end;
		float recovery_end;
		
		bool isUnstoppable(float progress);
		
		private:
		const float UNSTOPPABLE_AFTER = 0.5f; // Startup time scalar
	};
	
	extern std::vector<Punch> default_punches;
}
