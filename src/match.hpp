#pragma once

#include <vector>

#include "SFML/Window.hpp"
#include "config.hpp"
#include "level.hpp"
#include "player.hpp"
	
namespace fr {

class Match {
	public:
	Match(fr::ConfigFile config);

	void update(float dt, int global_time);
	void draw(sf::RenderWindow &window);
	void logState(int global_time);
	void logInputs(int global_time);
	void drawDebugGeometry(sf::RenderWindow &window);
	void drawDebugHurtboxes(sf::RenderWindow &window);
	void drawDebugHitboxes(sf::RenderWindow &window);
	void drawDebugInfo(sf::RenderWindow &window);

	private:
	Level level;
	Player player1;
	Player player2;
	float round_time = 180;
	sf::Font regular;
	sf::Font thin;
	sf::Text time_text;
	sf::Texture star;
	sf::Texture left_star;
	sf::Texture right_star;
	sf::Texture nostar;
	sf::Texture left_nostar;
	sf::Texture right_nostar;
};

} // namespace fr
