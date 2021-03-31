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
	float round_time = 180;
	sf::Text time_text;
	sf::Text p1_health_text;
	sf::Text p2_health_text;
};

} // namespace fr
