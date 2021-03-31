#include "match.hpp"

#include <vector>
#include <string>
#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "main.hpp"
#include "level.hpp"
#include "player.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "utils.hpp"

fr::Match::Match() {
	time_text.setFont(font);
	time_text.setCharacterSize(16);

	// Level
	sf::IntRect left(0, 128, 8, 48);
	sf::IntRect right(320 - 8, 128, 8, 48);
	level = Level{left, right};
	
	// Player 1
	Device device = (Device)config.getInt("player1_controls", "device", 0);
	std::vector<int> controls = {
		config.getInt("player1_controls", "left", -1),
		config.getInt("player1_controls", "right", -1),
		config.getInt("player1_controls", "up", -1),
		config.getInt("player1_controls", "down", -1),
		config.getInt("player1_controls", "jab", -1),
		config.getInt("player1_controls", "power", -1),
		config.getInt("player1_controls", "guard", -1),
		config.getInt("player1_controls", "pause", -1),
	};
	sf::Vector2f spawn_point = sf::Vector2f(20 + 64, 128);
	sf::Texture spritesheet;
	if (!spritesheet.loadFromFile("assets/placeholder.png"))
		std::cout << "Error loading texture" << std::endl;
	std::vector<fr::Animation> animations;
	fr::Stats stats;

	players.push_back(Player(1, 1, device, controls, spawn_point, spritesheet,
			animations, stats));
			
	// Player 2
	device = (Device)config.getInt("player2_controls", "device", 0);
	controls = {
		config.getInt("player2_controls", "left", -1),
		config.getInt("player2_controls", "right", -1),
		config.getInt("player2_controls", "up", -1),
		config.getInt("player2_controls", "down", -1),
		config.getInt("player2_controls", "jab", -1),
		config.getInt("player2_controls", "power", -1),
		config.getInt("player2_controls", "guard", -1),
		config.getInt("player2_controls", "pause", -1),
	};
	spawn_point = sf::Vector2f(320 - 20 - 64 * 2, 128);

	players.push_back(Player(2, -1, device, controls, spawn_point, spritesheet,
			animations, stats));

	// Debug
	p1_health_text.setFont(font);
	p1_health_text.setCharacterSize(12);

	p2_health_text.setFont(font);
	p2_health_text.setCharacterSize(12);
}

void fr::Match::update() {
	if (round_time > 0)
		round_time -= dt;

	int minutes = (int)round_time / 60;
	int seconds = (int)round_time % 60;
	time_text.setString(std::to_string(minutes) + ":" + std::to_string(seconds));

	players[0].update(level.geometry(), players[1]);
	players[1].update(level.geometry(), players[0]);

	// Debug
	if (config.getBool("debug", "draw_info", false)) {
		p1_health_text.setString("HP: " + std::to_string(players[0].health) + "/"
				+ std::to_string(players[0].max_health));

		p2_health_text.setString("HP: " + std::to_string(players[1].health) + "/"
				+ std::to_string(players[1].max_health));
	}
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	players[0].draw(window);
	players[1].draw(window);

	// TODO Un-hardcode
	int middle = 160 - time_text.getLocalBounds().width / 2;
	time_text.setPosition(middle, 16);
	window.draw(time_text);

	// Debug
	if (config.getBool("debug", "draw_info", false)) {
		p1_health_text.setPosition(8, 240 - 16);
		window.draw(p1_health_text);

		int right = 320 - 8 - p2_health_text.getLocalBounds().width;
		p2_health_text.setPosition(right, 240 - 16);
		window.draw(p2_health_text);
	}
}
