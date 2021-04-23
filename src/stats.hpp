#pragma once

#include "SFML/System.hpp"

namespace fr {

struct Stats {
	int max_health = 20;
	float health_regen = 1;
	float regen_rate = 1;

	float walk_speed = 92;

	// Pull (dodge type)
	int pull_damage = 0;
	float tt_pull = 0.1f;
	float pull_active = 0.15f;
	float pull_recovery = 0.2f;
	sf::Vector2f pull_offset = sf::Vector2f(0, 0);

	// Slip (dodge type)
	int slip_damage = 0;
	float tt_slip = 0.1f;
	float slip_active = 0.075f;
	float slip_recovery = 0.1f;
	sf::Vector2f slip_offset = sf::Vector2f(0, 0);

	// Duck (dodge type)
	int duck_damage = 1;
	float tt_duck = 0.15f;
	float duck_active = 0.2f;
	float duck_recovery = 0.1f;
	sf::Vector2f duck_offset = sf::Vector2f(0, 0);

	//int escape_damage = 1;
	//float escape_range = 4;

	float guard_recovery = 0.05f;
};

} // namespace fr
