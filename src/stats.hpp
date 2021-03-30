#pragma once

namespace fr {

struct Stats {
	int max_health = 20;
	float health_regen = 1;

	float walk_speed = 100;

	int pull_damage = 0;
	float tt_pull = 0.1f;
	float pull_active = 0.15f;
	float pull_recovery = 0.2f;
	int pull_distance = -50;

	int weave_damage = 0;
	float tt_weave = 0.1f;
	float weave_active = 0.075f;
	float weave_recovery = 0.1f;

	int duck_damage = 1;
	float tt_duck = 0.15f;
	float duck_active = 0.2f;
	float duck_recovery = 0.1f;

	//int escape_damage = 1;
	//float escape_range = 4;

	float guard_recovery = 0.05f;
};

} // namespace fr
