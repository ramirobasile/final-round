#pragma once

namespace fr {

struct Stats {
	Stats(int punches, int body, int feet);

	const float BODY_HEAD_HP_RATIO = 0.5f;
	const float BODY_GUARD_HP_RATIO = 0.33f;
	const float STAGGER_THRESHOLD = 0.1f;
	const float CONCUSSION_THRESHOLD = 0.33f;

	int punches;
	int body;
	int feet;

	int max_body_hp = 100;
	int max_health = 20;
	float walk_speed = 100;
	float tt_weave = 0.1f;
	float tt_pull = 0.125f;
	float tt_duck = 0.15f;
};

} // namespace fr
