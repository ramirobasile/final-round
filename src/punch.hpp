#pragma once

#include <vector>

#include <toml++/toml.h>
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
		
		bool isStartingUp(float progress);
		bool isUnstoppable(float progress);
		bool isActive(float progress);
		bool isRecovering(float progress);
		bool isDone(float progress);
		sf::IntRect getHitbox(sf::Vector2f relative_to);
	};
}
