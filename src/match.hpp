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
	Player player1;
	Player player2;
	float round_time = 180;
	sf::Text time_text;
};

} // namespace fr
