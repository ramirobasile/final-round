#include "match.hpp"

#include <vector>
#include <string>

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
	// Level
	sf::IntRect left(0, 128, 20, 64);
	sf::IntRect right(300, 128, 20, 64);
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
	spritesheet.loadFromFile("assets/placeholder.png");
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
}

void fr::Match::update() {
	players[0].update(level.geometry(), players[1]);
	players[1].update(level.geometry(), players[0]);
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	players[0].draw(window);
	players[1].draw(window);
}
