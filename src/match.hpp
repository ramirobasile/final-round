#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <toml++/toml.h>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "level.hpp"
#include "player.hpp"
#include "input.hpp"
#include "stats.hpp"
	
namespace fr {
	class Match {
		public:
		Match();

		void update();
		void draw(sf::RenderWindow &window);

		private:
		Level level;
		std::vector<Player> players;
	};
}
