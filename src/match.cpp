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
#include "punch.hpp"
#include "dodge.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "sprite.hpp"
#include "utils.hpp"

// Debug
sf::Text p1_info;
sf::Text p2_info;

fr::Match::Match(fr::ConfigFile config) {
	std::string prefix = "../../share/games/final-round/assets/";

	if (!regular.loadFromFile(prefix + "unscii-8.pcf"));
	if (!thin.loadFromFile(prefix + "unscii-8-thin.pcf"));

	star.loadFromFile(prefix + "star.png");
	left_star.loadFromFile(prefix + "left_star.png");
	right_star.loadFromFile(prefix + "right_star.png");
	nostar.loadFromFile(prefix + "nostar.png");
	left_nostar.loadFromFile(prefix + "left_nostar.png");
	right_nostar.loadFromFile(prefix + "right_nostar.png");

	// Level
	sf::Texture ring;
	if (!ring.loadFromFile(prefix + "ring.png"));

	sf::FloatRect left(8, 112, 8, 64);
	sf::FloatRect right(320 - 16, 112, 8, 64);

	level = Level(left, right, ring);
	
	// Player 1
	Devices device = (Devices)config.getInt("player1_controls", "device", 0);
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
	l_spritesheet.loadFromFile(prefix + "red_l_spritesheet.png");
	sf::Texture r_spritesheet;
	r_spritesheet.loadFromFile(prefix + "red_r_spritesheet.png");

	std::vector<Animations> walk_chain = {
		Animations::idle,
		Animations::walk,
		Animations::walk_head,
		Animations::walk_body
	 };
	std::vector<Animation> animations {
		Animation(1, true, std::vector<Animations>{}),
		Animation(1, true, std::vector<Animations>{}),
		Animation(1, true, std::vector<Animations>{}),

		Animation(8, true, walk_chain),
		Animation(8, true, walk_chain),
		Animation(8, true, walk_chain),

		Animation(5, false, std::vector<Animations>{}),
		Animation(5, false, std::vector<Animations>{}),
		Animation(7, false, std::vector<Animations>{}),
		Animation(7, false, std::vector<Animations>{}),
		Animation(9, false, std::vector<Animations>{}),
		Animation(9, false, std::vector<Animations>{}),
		Animation(6, false, std::vector<Animations>{}),
		Animation(6, false, std::vector<Animations>{}),
		
		Animation(1, true, std::vector<Animations>{}),
		Animation(1, true, std::vector<Animations>{}),
		Animation(1, true, std::vector<Animations>{}),
		
		Animation(1, true, std::vector<Animations>{}),
		Animation(1, true, std::vector<Animations>{}),
		
		Animation(1, true, std::vector<Animations>{}),
	};
	Sprite sprite(l_spritesheet, r_spritesheet, animations,
			sf::Vector2i(128, 128), 16);

	Stats stats;

	player1 = Player(1, "MKEY", 1, device, controls, spawn_point, sprite, 
			stats, default_punches, default_dodges);
			
	// Player 2
	device = (Devices)config.getInt("player2_controls", "device", 0);
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

	l_spritesheet.loadFromFile(prefix + "blue_l_spritesheet.png");
	r_spritesheet.loadFromFile(prefix + "blue_l_spritesheet.png");
	sprite = Sprite(l_spritesheet, r_spritesheet, animations, 
			sf::Vector2i(128, 128), 16);

	player2 = Player(2, "FLIP", -1, device, controls, spawn_point, sprite, 
			stats, default_punches, default_dodges);

	// UI
	time_text.setFont(regular);
	time_text.setCharacterSize(8);
	p1_alias_text.setFont(regular);
	p1_alias_text.setCharacterSize(8);
	p1_alias_text.setPosition(8, 16);
	p1_alias_text.setString(player1.alias);
	p2_alias_text.setFont(regular);
	p2_alias_text.setCharacterSize(8);
	p2_alias_text.setString(player2.alias);
	p2_alias_text.setPosition(312 - p2_alias_text.getLocalBounds().width , 16);

	// Debug
	p1_info.setFont(thin);
	p1_info.setCharacterSize(8);
	p2_info.setFont(thin);
	p2_info.setCharacterSize(8);
}

void fr::Match::update(float dt) {
	if (round_time > 0)
		round_time -= dt;

	std::string minutes = std::to_string((int)round_time / 60);
	std::string seconds = fillString(std::to_string((int)round_time % 60), 2, '0');
	time_text.setString(minutes + ":" + seconds);

	player1.update(dt, level.getGeometry(), player2);
	player2.update(dt, level.getGeometry(), player1);
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	player1.draw(window);
	player2.draw(window);

	// TODO Un-hardcode
	int middle = 160 - time_text.getLocalBounds().width / 2;
	time_text.setPosition(middle, 16);
	window.draw(time_text);

	window.draw(p1_alias_text);
	window.draw(p2_alias_text);

	// Player 1 health
	for (int i = 0; i < player1.max_health; i+=2) {
		sf::Sprite sprite(nostar);
		if (i ==  player1.max_health - 1)
			sprite = sf::Sprite(right_nostar);
		sprite.setPosition(134 - sprite.getTextureRect().width * (i / 2) - i, 32);
		window.draw(sprite);
	}
	for (int i = 0; i < player1.health; i+=2) {
		sf::Sprite sprite(star);
		if (i ==  player1.health - 1)
			sprite = sf::Sprite(right_star);
		sprite.setPosition(134 - sprite.getTextureRect().width * (i / 2) - i, 32);
		window.draw(sprite);
	}

	// Player 2 health
	for (int i = 0; i < player2.max_health; i+=2) {
		sf::Sprite sprite(nostar);
		if (i ==  player2.max_health - 1)
			sprite = sf::Sprite(left_nostar);
		sprite.setPosition(170 + sprite.getTextureRect().width * (i / 2) + i, 32);
		window.draw(sprite);
	}
	for (int i = 0; i < player2.health; i+=2) {
		sf::Sprite sprite(star);
		if (i ==  player2.health - 1)
			sprite = sf::Sprite(left_star);
		sprite.setPosition(170 + sprite.getTextureRect().width * (i / 2) + i, 32);
		window.draw(sprite);
	}
}


///////////////////////////////////////////////////////////
// Debug methods
///////////////////////////////////////////////////////////
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
	p1_str += "\nGUA:" + std::to_string((int)player1.state.guard);
	//p1_str += "\nPCH:" + std::to_string((int)player1.state.punch);
	for (int i = 0; i < player1.inputs.size(); ++i)
		p1_str += "\n>" + (std::string)player1.inputs[i];
	p1_info.setString(p1_str);

	std::string p2_str;
	p2_str += "HP:" + std::to_string(player2.health) + "/"
			+ std::to_string(player2.max_health);
	p2_str += "\nMOV:" + std::to_string((int)player2.state.movement);
	p2_str += "\nGUA:" + std::to_string((int)player2.state.guard);
	//p2_str += "\nPCH:" + std::to_string((int)player2.state.punch);
	for (int i = 0; i < player2.inputs.size(); ++i)
		p2_str += "\n>" + (std::string)player2.inputs[i];
	p2_info.setString(p2_str);

	p1_info.setPosition(8, 48);
	window.draw(p1_info);
	p2_info.setPosition(170, 48);
	window.draw(p2_info);
}
