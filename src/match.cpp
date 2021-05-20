#include "match.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "direction.hpp"
#include "dodge.hpp"
#include "input.hpp"
#include "level.hpp"
#include "player.hpp"
#include "sprite.hpp"
#include "stats.hpp"
#include "utils.hpp"

fr::Match::Match(fr::ConfigFile config) {
	std::string prefix = "../share/games/final-round/assets/";

	regular.loadFromFile(prefix + "unscii-8.pcf");
	thin.loadFromFile(prefix + "unscii-8-thin.pcf");

	star.loadFromFile(prefix + "star.png");
	left_star.loadFromFile(prefix + "left_star.png");
	right_star.loadFromFile(prefix + "right_star.png");
	nostar.loadFromFile(prefix + "nostar.png");
	left_nostar.loadFromFile(prefix + "left_nostar.png");
	right_nostar.loadFromFile(prefix + "right_nostar.png");

	// Level
	sf::Texture ring;
	ring.loadFromFile(prefix + "ring.png");

	sf::FloatRect left(8, 112, 8, 64);
	sf::FloatRect right(320 - 16, 112, 8, 64);

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
		config.getInt("player1_controls", "x", -1),
		config.getInt("player1_controls", "y", -1),
		config.getInt("player1_controls", "l", -1),
		config.getInt("player1_controls", "r", -1),
		config.getInt("player1_controls", "pause", -1),
	};

	sf::Texture r_spritesheet, l_spritesheet;
	r_spritesheet.loadFromFile(prefix + "red_right_spritesheet.png");
	l_spritesheet.loadFromFile(prefix + "red_left_spritesheet.png");

	// TODO Make all of this const somewhere else
	std::vector<Animation> movement_anims {
		Animation(0, 1, true),		// idle
		Animation(1, 1, true),		// idle_head
		Animation(2, 1, true),		// idle_body
		Animation(3, 8, true),		// walk
		Animation(4, 8, true),		// walk_head
		Animation(5, 8, true),		// walk_body
		Animation(9, 1, false),		// hit_head
		Animation(10, 1, false),	// hit_body
		Animation(11, 1, false),	// ko
	};

	Stats stats;

	player1 = Player(Direction::right, device, controls, stats, r_spritesheet,
			l_spritesheet, movement_anims, 0);
	player1.position = sf::Vector2f(16 + 64, 112);
			
	// Player 2
	device = (Device)config.getInt("player2_controls", "device", 0);
	controls = {
		config.getInt("player2_controls", "left", -1),
		config.getInt("player2_controls", "right", -1),
		config.getInt("player2_controls", "up", -1),
		config.getInt("player2_controls", "down", -1),
		config.getInt("player2_controls", "a", -1),
		config.getInt("player2_controls", "b", -1),
		config.getInt("player2_controls", "x", -1),
		config.getInt("player2_controls", "y", -1),
		config.getInt("player2_controls", "l", -1),
		config.getInt("player2_controls", "r", -1),
		config.getInt("player2_controls", "pause", -1),
	};

	r_spritesheet.loadFromFile(prefix + "blue_right_spritesheet.png");
	l_spritesheet.loadFromFile(prefix + "blue_left_spritesheet.png");

	player2 = Player(Direction::left, device, controls, stats, r_spritesheet,
			l_spritesheet, movement_anims, 1);
	player2.position = sf::Vector2f(320 - 16 - 64 * 2, 112);

	// UI
	time_text.setFont(regular);
	time_text.setCharacterSize(8);
	p1_alias_text.setFont(regular);
	p1_alias_text.setCharacterSize(8);
	p1_alias_text.setPosition(8, 16);
	p1_alias_text.setString("PLAYER 1");
	p2_alias_text.setFont(regular);
	p2_alias_text.setCharacterSize(8);
	p2_alias_text.setString("PLAYER 2");
	p2_alias_text.setPosition(312 - p2_alias_text.getLocalBounds().width , 16);
}

void fr::Match::update(float dt) {
	float distance = vector2Distance(player1.position, player2.position);

	player1.update(distance, dt);
	player2.update(distance, dt);

	// Move players and resolve collisions
	std::vector<sf::FloatRect> geometry = level.getGeometry();

	updatePosition(player1.position, player1.getVelocity(), dt);

	resolveCollision(player1.position, player1.getBounds(), player2.getBounds());
	for (unsigned int i = 0; i < geometry.size(); ++i)
		resolveCollision(player1.position, player1.getBounds(), geometry[i]);

	updatePosition(player2.position, player2.getVelocity(), dt);

	resolveCollision(player2.position, player2.getBounds(), player1.getBounds());
	for (unsigned int i = 0; i < geometry.size(); ++i)
		resolveCollision(player2.position, player2.getBounds(), geometry[i]);

	// Hit each other
	sf::FloatRect hitbox = player1.punch.getHitbox(player1.getBounds(), player1.getDirection());
	if (player1.punch.isActive() &&
			(hitbox.intersects(player2.getHeadHurtbox())
			|| hitbox.intersects(player2.getBodyHurtbox()))) {
		player2.takeHit(player1.punch.getHit());
		player1.punch.interrupt();
	}

	hitbox = player2.punch.getHitbox(player2.getBounds(), player2.getDirection());
	if (player2.punch.isActive() &&
			(hitbox.intersects(player1.getHeadHurtbox())
			|| hitbox.intersects(player1.getBodyHurtbox()))) {
		player1.takeHit(player2.punch.getHit());
		player2.punch.interrupt();
	}

	// Stuff each other
	sf::FloatRect clearbox = player1.punch.getClearbox(player1.getBounds(), player1.getDirection());
	if (!player1.punch.isRecovering() && clearbox.intersects(player2.getBounds()))
		player1.punch.interrupt();

	clearbox = player2.punch.getClearbox(player2.getBounds(), player2.getDirection());
	if (!player2.punch.isRecovering() && clearbox.intersects(player1.getBounds()))
		player2.punch.interrupt();

	// Round timer
	if (round_time > 0)
		round_time -= dt;

	std::string minutes = std::to_string((int)round_time / 60);
	std::string seconds = fillString(std::to_string((int)round_time % 60), 2, '0');
	time_text.setString(minutes + ":" + seconds);
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	player1.draw(window);
	player2.draw(window);

	// TODO Make positions relative to screen size
	int middle = 160 - time_text.getLocalBounds().width / 2;
	time_text.setPosition(middle, 16);
	window.draw(time_text);

	window.draw(p1_alias_text);
	window.draw(p2_alias_text);

	// Player 1 health
	for (int i = 0; i < player1.getMaxHealth(); i+=2) {
		sf::Sprite sprite(nostar);
		if (i == player1.getMaxHealth() - 1)
			sprite = sf::Sprite(right_nostar);
		sprite.setPosition(134 - sprite.getTextureRect().width * (i / 2) - i, 32);
		window.draw(sprite);
	}
	for (int i = 0; i < player1.getHealth(); i+=2) {
		sf::Sprite sprite(star);
		if (i ==  player1.getHealth() - 1)
			sprite = sf::Sprite(right_star);
		sprite.setPosition(134 - sprite.getTextureRect().width * (i / 2) - i, 32);
		window.draw(sprite);
	}

	// Player 2 health
	for (int i = 0; i < player2.getMaxHealth(); i+=2) {
		sf::Sprite sprite(nostar);
		if (i ==  player2.getMaxHealth() - 1)
			sprite = sf::Sprite(left_nostar);
		sprite.setPosition(170 + sprite.getTextureRect().width * (i / 2) + i, 32);
		window.draw(sprite);
	}
	for (int i = 0; i < player2.getHealth(); i+=2) {
		sf::Sprite sprite(star);
		if (i ==  player2.getHealth() - 1)
			sprite = sf::Sprite(left_star);
		sprite.setPosition(170 + sprite.getTextureRect().width * (i / 2) + i, 32);
		window.draw(sprite);
	}
}
