#include "match.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "main.hpp"
#include "level.hpp"
#include "player.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "utils.hpp"

// Debug
sf::Text p1_info;
sf::Text p2_info;

fr::Match::Match() {
	time_text.setFont(font);
	time_text.setCharacterSize(32);

	// Level
	sf::Texture ring;
	if (!ring.loadFromFile("assets/ring.png"))
		std::cout << "Error loading texture" << std::endl;

	sf::IntRect left(0, 224, 16, 128);
	sf::IntRect right(640 - 16, 224, 16, 128);

	level = Level{left, right, ring};
	
	// Player 1
	Device device = (Device)config.getInt("player1_controls", "device", 0);
	std::vector<int> controls = {
		config.getInt("player1_controls", "left", -1),
		config.getInt("player1_controls", "right", -1),
		config.getInt("player1_controls", "up", -1),
		config.getInt("player1_controls", "down", -1),
		config.getInt("player1_controls", "a", -1),
		config.getInt("player1_controls", "b", -1),
		config.getInt("player1_controls", "pause", -1),
	};
	sf::Vector2f spawn_point = sf::Vector2f(32 + 128, 224);
	sf::Texture spritesheet;
	if (!spritesheet.loadFromFile("assets/placeholder.png"))
		std::cout << "Error loading texture" << std::endl;
	std::vector<fr::Animation> animations;
	fr::Stats stats;

	player1 = Player(1, 1, device, controls, spawn_point, spritesheet,
			animations, stats);
			
	// Player 2
	device = (Device)config.getInt("player2_controls", "device", 0);
	controls = {
		config.getInt("player2_controls", "left", -1),
		config.getInt("player2_controls", "right", -1),
		config.getInt("player2_controls", "up", -1),
		config.getInt("player2_controls", "down", -1),
		config.getInt("player2_controls", "a", -1),
		config.getInt("player2_controls", "b", -1),
		config.getInt("player2_controls", "pause", -1),
	};
	spawn_point = sf::Vector2f(640 - 32 - 128 * 2, 224);

	player2 = Player(2, -1, device, controls, spawn_point, spritesheet,
			animations, stats);

	// Debug
	p1_info.setFont(font);
	p1_info.setCharacterSize(16);
	p2_info.setFont(font);
	p2_info.setCharacterSize(16);
}

void fr::Match::update() {
	if (round_time > 0)
		round_time -= dt;

	std::string minutes = std::to_string((int)round_time / 60);
	std::stringstream seconds;
	seconds << std::setw(2) << std::setfill('0') << (int)round_time % 60;
	time_text.setString(minutes + ":" + seconds.str());

	player1.update(level.geometry(), player2);
	player2.update(level.geometry(), player1);

	// Debug
	if (config.getBool("debug", "draw_info", false)) {
		std::string p1_str;
		p1_str += "HP: " + std::to_string(player1.health) + "/"
				+ std::to_string(player1.max_health);
		p1_str += "\nMovement: " + std::to_string((int)player1.state.movement);
		p1_str += "\nPunch: " + std::to_string(player1.state.punch.progress);
		p1_str += "\nInputs:";
		for (int i = 0; i < player1.inputs.size(); ++i)
			p1_str += "\n* " + (std::string)player1.inputs[i];
		p1_info.setString(p1_str);

		std::string p2_str;
		p2_str += "HP: " + std::to_string(player2.health) + "/"
				+ std::to_string(player2.max_health);
		p2_str += "\nMovement: " + std::to_string((int)player2.state.movement);
		p2_str += "\nPunch: " + std::to_string(player2.state.punch.progress);
		p2_str += "\nInputs:";
		for (int i = 0; i < player2.inputs.size(); ++i)
			p2_str += "\n* " + (std::string)player2.inputs[i];
		p2_info.setString(p2_str);
	}
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	player1.draw(window);
	player2.draw(window);

	// TODO Un-hardcode
	int middle = 320 - time_text.getLocalBounds().width / 2;
	time_text.setPosition(middle, 32);
	window.draw(time_text);

	// Debug
	if (config.getBool("debug", "draw_info", false)) {
		p1_info.setPosition(16, 64);
		window.draw(p1_info);
		p2_info.setPosition(640 - 16 - p2_info.getLocalBounds().width, 64);
		window.draw(p2_info);
	}
}
