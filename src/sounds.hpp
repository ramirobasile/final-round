#pragma once

#include "SFML/Audio.hpp"

namespace fr {

struct Sounds {
	sf::Sound step;
	sf::Sound dodge;
	sf::Sound block;
	sf::Sound hit;
	sf::Sound ko;
	sf::Sound ui_move;
};

} // namespace fr
