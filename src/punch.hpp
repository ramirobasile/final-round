#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

namespace fr {
	const float UNSTOPPABLE_AFTER = 0.5f; // Startup time scalar

	struct Punch {
		Control control;
		bool is_lead_handed;
		bool is_body;
		int damage;
		int body_hp_cost;
		float hitbox_begin;
		float hitbox_end;
		float recovery_end;
		sf::IntRect hitbox;
		
		bool isStartingUp(float progress) const;
		bool isUnstoppable(float progress) const;
		bool isActive(float progress) const;
		bool isRecovering(float progress) const;
		bool isDone(float progress) const;
		sf::IntRect getHitbox(sf::Vector2f relative_to) const;
	};

	extern std::vector<Punch> default_punches;
}
