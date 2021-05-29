#pragma once

#include "animation.hpp"

namespace fr {

struct Animations {
	Animation idle;
	Animation idle_guard_head;
	Animation idle_guard_body;
	Animation walk;
	Animation walk_guard_head;
	Animation walk_guard_body;
	Animation hit_head;
	Animation hit_body;
	Animation ko;
};

} // namespace fr
