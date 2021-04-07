#include "match.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "level.hpp"
#include "player.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "sprite.hpp"
#include "utils.hpp"

// Debug
sf::Text p1_info;
sf::Text p2_info;

fr::Match::Match(fr::ConfigFile config) {
	if (!regular.loadFromFile("assets/unscii-8.pcf"));
	if (!thin.loadFromFile("assets/unscii-8-thin.pcf"));

	star.loadFromFile("assets/star.png");
	left_star.loadFromFile("assets/left_star.png");
	right_star.loadFromFile("assets/right_star.png");
	nostar.loadFromFile("assets/nostar.png");
	left_nostar.loadFromFile("assets/left_nostar.png");
	right_nostar.loadFromFile("assets/right_nostar.png");

	// Level
	sf::Texture ring;
	if (!ring.loadFromFile("assets/ring.png"));

	sf::IntRect left(8, 112, 8, 64);
	sf::IntRect right(320 - 16, 112, 8, 64);

	level = Level(left, right, ring);
	
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

	sf::Vector2f spawn_point = sf::Vector2f(16 + 64, 112);

	sf::Texture l_spritesheet;
	l_spritesheet.loadFromFile("assets/red_l_spritesheet.png");
	sf::Texture r_spritesheet;
	r_spritesheet.loadFromFile("assets/red_r_spritesheet.png");
	std::vector<Animation> animations {
		Animation(1, true),
		Animation(1, true),
		Animation(1, true),

		Animation(4, true),
		Animation(4, true),
		Animation(4, true),
		Animation(4, true),
		Animation(4, true),
		Animation(4, true),

		Animation(4, false),
	};
	Sprite sprite(l_spritesheet, r_spritesheet, animations,
			sf::Vector2i(128, 128), 12);

	Stats stats;

	player1 = Player(1, 1, device, controls, spawn_point, sprite, stats);
			
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

	spawn_point = sf::Vector2f(320 - 16 - 64 * 2, 112);

	l_spritesheet.loadFromFile("assets/blue_l_spritesheet.png");
	r_spritesheet.loadFromFile("assets/blue_l_spritesheet.png");
	sprite = Sprite(l_spritesheet, r_spritesheet, animations, 
			sf::Vector2i(128, 128), 12);

	player2 = Player(2, -1, device, controls, spawn_point, sprite, stats);

	// UI
	time_text.setFont(regular);
	time_text.setCharacterSize(8);

	// Debug
	p1_info.setFont(thin);
	p1_info.setCharacterSize(8);
	p2_info.setFont(thin);
	p2_info.setCharacterSize(8);
}

void fr::Match::update(float dt, int global_time) {
	if (round_time > 0)
		round_time -= dt;

	std::string minutes = std::to_string((int)round_time / 60);
	std::string seconds = fillString(std::to_string((int)round_time % 60), 2, '0');
	time_text.setString(minutes + ":" + seconds);

	player1.update(dt, global_time, level.getGeometry(), player2);
	player2.update(dt, global_time, level.getGeometry(), player1);
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	player1.draw(window);
	player2.draw(window);

	// TODO Un-hardcode
	int middle = 160 - time_text.getLocalBounds().width / 2;
	time_text.setPosition(middle, 12);
	window.draw(time_text);
	// Player 1 health
	for (int i = 0; i < player1.max_health; i+=2) {
		sf::Sprite sprite(nostar);
		if (i ==  player1.max_health - 1)
			sprite = sf::Sprite(right_nostar);
		sprite.setPosition(136 - sprite.getTextureRect().width * (i / 2) - i, 24);
		window.draw(sprite);
	}
	for (int i = 0; i < player1.health; i+=2) {
		sf::Sprite sprite(star);
		if (i ==  player1.health - 1)
			sprite = sf::Sprite(right_star);
		sprite.setPosition(136 - sprite.getTextureRect().width * (i / 2) - i, 24);
		window.draw(sprite);
	}

	// Player 2 health
	for (int i = 0; i < player2.max_health; i+=2) {
		sf::Sprite sprite(nostar);
		if (i ==  player2.max_health - 1)
			sprite = sf::Sprite(left_nostar);
		sprite.setPosition(172 + sprite.getTextureRect().width * (i / 2) + i, 24);
		window.draw(sprite);
	}
	for (int i = 0; i < player2.health; i+=2) {
		sf::Sprite sprite(star);
		if (i ==  player2.health - 1)
			sprite = sf::Sprite(left_star);
		sprite.setPosition(172 + sprite.getTextureRect().width * (i / 2) + i, 24);
		window.draw(sprite);
	}
}


///////////////////////////////////////////////////////////
// Debug methods
///////////////////////////////////////////////////////////
void fr::Match::logState(int global_time) {
	player1.logState(global_time);
	player2.logState(global_time);
}
void fr::Match::logInputs(int global_time) {
	player1.logInputs(global_time);
	player2.logInputs(global_time);
}

void fr::Match::drawDebugGeometry(sf::RenderWindow &window) {
	level.drawDebugGeometry(window);
	player1.drawDebugGeometry(window);
	player2.drawDebugGeometry(window);
}

void fr::Match::drawDebugHurtboxes(sf::RenderWindow &window) {
	player1.drawDebugHurtboxes(window);
	player2.drawDebugHurtboxes(window);
}

void fr::Match::drawDebugHitboxes(sf::RenderWindow &window) {
	player1.drawDebugHitboxes(window);
	player2.drawDebugHitboxes(window);
}

void fr::Match::drawDebugInfo(sf::RenderWindow &window) {
	std::string p1_str;
	p1_str += "HP:" + std::to_string(player1.health) + "/"
			+ std::to_string(player1.max_health);
	p1_str += "\nMOV:" + std::to_string((int)player1.state.movement);
	//p1_str += "\nPCH:" + std::to_string((int)player1.state.punch.progress);
	for (int i = 0; i < player1.inputs.size(); ++i)
		p1_str += "\n>" + (std::string)player1.inputs[i];
	p1_info.setString(p1_str);

	std::string p2_str;
	p2_str += "HP:" + std::to_string(player2.health) + "/"
			+ std::to_string(player2.max_health);
	p2_str += "\nMOV:" + std::to_string((int)player2.state.movement);
	//p2_str += "\nPCH:" + std::to_string((int)player2.state.punch.progress);
	for (int i = 0; i < player2.inputs.size(); ++i)
		p2_str += "\n>" + (std::string)player2.inputs[i];
	p2_info.setString(p2_str);

	p1_info.setPosition(8, 48);
	window.draw(p1_info);
	p2_info.setPosition(176, 48);
	window.draw(p2_info);
}
