#pragma once

#include <vector>

#include "animation.hpp"
#include "dodge.hpp"
#include "punch.hpp"
#include "stats.hpp"

namespace fr {

struct Character {
	Stats stats;
	std::vector<Punch> punches;
	std::vector<Dodge> dodges;
	std::vector<Animation> animations;
	
	enum class Name {
		base,
	};
};

extern std::vector<Character> characters;

}; // namespace fr
