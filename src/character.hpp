#pragma once

#include <vector>

#include "SFML/Audio.hpp"
#include "animations.hpp"
#include "dodge.hpp"
#include "punch.hpp"
#include "stats.hpp"

namespace fr {

struct Character {
	Character(sf::Sound jab_sound, sf::Sound power_sound); // Default character
	
	Stats stats;
	std::vector<Punch> punches;
	std::vector<Dodge> dodges;
	Animations animations;
};

}; // namespace fr
