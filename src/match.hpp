#pragma once

#include <string>
#include <vector>

#include "SFML/Window.hpp"
#include "config.hpp"
#include "health_bar.hpp"
#include "level.hpp"
#include "player.hpp"
	
namespace fr {

class Match {
	public:
	Match(fr::ConfigFile config);

	void update(float dt);
	void draw(sf::RenderWindow &window, float dt);

	private:
	const float MIN_SHAKE_STUN = 0.1f;
	const float SHAKE_SPEED = 10;

	std::vector<int> getControls(std::string section);
	
	ConfigFile config;
	sf::Texture ring;
	sf::Texture star;
	sf::Texture left_star;
	sf::Texture right_star;
	sf::Texture nostar;
	sf::Texture left_nostar;
	sf::Texture right_nostar;
	sf::Font regular;
	sf::Font thin;
	sf::SoundBuffer step;
	sf::SoundBuffer dodge;
	sf::SoundBuffer jab;
	sf::SoundBuffer power;
	sf::SoundBuffer block;
	sf::SoundBuffer hit;
	sf::SoundBuffer ko;
	sf::SoundBuffer ui_move;
	Level level;
	Player player1;
	Player player2;
	float round_time = 180;
	sf::Text p1_alias_text;
	sf::Text p2_alias_text;
	sf::Text time_text;
	HealthBar p1_health_bar;
	HealthBar p2_health_bar;
	float shake = 0;
};

} // namespace fr
