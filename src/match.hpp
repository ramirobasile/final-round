#pragma once

#include <vector>

#include "SFML/Window.hpp"
#include "level.hpp"
#include "player.hpp"
	
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
