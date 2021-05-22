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
#include "health_bar.hpp"
#include "input.hpp"
#include "level.hpp"
#include "player.hpp"
#include "sprite.hpp"
#include "stats.hpp"
#include "stun.hpp"
#include "utils.hpp"

fr::Match::Match(fr::ConfigFile config) : config(config) {
	// Load assets
	std::string prefix = "../share/games/final-round/assets/";

	ring.loadFromFile(prefix + "ring.png");
	star.loadFromFile(prefix + "star.png");
	left_star.loadFromFile(prefix + "left_star.png");
	right_star.loadFromFile(prefix + "right_star.png");
	nostar.loadFromFile(prefix + "nostar.png");
	left_nostar.loadFromFile(prefix + "left_nostar.png");
	right_nostar.loadFromFile(prefix + "right_nostar.png");

	regular.loadFromFile(prefix + "unscii-8.pcf");
	thin.loadFromFile(prefix + "unscii-8-thin.pcf");
	
	// Level
	sf::FloatRect left(8, 112, 8, 64);
	sf::FloatRect right(320 - 16, 112, 8, 64);

	level = Level(left, right, ring);
	
	// TODO Make all of this const somewhere else
	std::vector<Animation> animations {
		Animation(1, false),	// idle
		Animation(1, false),	// idle_guard_head
		Animation(1, false),	// idle_guard_body
		Animation(8, true),		// walk
		Animation(8, true),		// walk_guard_head
		Animation(8, true),		// walk_guard_body
		Animation(6, false),	// pull
		Animation(6, false),	// slip
		Animation(6, false),	// duck
		Animation(1, false),	// hit_head
		Animation(1, false),	// hit_body
		Animation(1, false),	// ko
		Animation(5, false),	// jab
		Animation(5, false),	// jab_body
		Animation(7, false),	// cross
		Animation(7, false),	// cross_body
		Animation(8, false),	// upper
		Animation(8, false),	// upper_body
		Animation(9, false),	// hook
		Animation(9, false),	// hook_body
	};
	
	// Player 1
	Device device = (Device)config.getInt("player1_controls", "device", 0);

	sf::Texture r_spritesheet, l_spritesheet;
	r_spritesheet.loadFromFile(prefix + "red_right_spritesheet.png");
	l_spritesheet.loadFromFile(prefix + "red_left_spritesheet.png");

	Stats stats;

	player1 = Player(Direction::right, device, getControls("player1_controls"), 
			stats, r_spritesheet, l_spritesheet, animations, 0);
	player1.position = sf::Vector2f(16 + 64, 112);
			
	// Player 2
	device = (Device)config.getInt("player2_controls", "device", 0);
	
	r_spritesheet.loadFromFile(prefix + "blue_right_spritesheet.png");
	l_spritesheet.loadFromFile(prefix + "blue_left_spritesheet.png");

	player2 = Player(Direction::left, device, getControls("player2_controls"), 
			stats, r_spritesheet, l_spritesheet, animations, 1);
	player2.position = sf::Vector2f(320 - 16 - 64 * 2, 112);

	// UI elements
	// TODO Make values relative to screen size
	time_text = sf::Text("", regular, 8);
	
	p1_alias_text = sf::Text("PLAYER 1", regular, 8);
	p1_alias_text.setPosition(8, 16);
	p2_alias_text = sf::Text("PLAYER 2", regular, 8);
	p2_alias_text.setPosition(312 - p2_alias_text.getLocalBounds().width , 16);
	
	p1_health_bar = HealthBar(sf::Vector2f(134, 30), Direction::left, star, 
			left_star, right_star, nostar, left_nostar, right_nostar);
			
	p2_health_bar = HealthBar(sf::Vector2f(170, 30), Direction::right, star, 
			left_star, right_star, nostar, left_nostar, right_nostar);
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

void fr::Match::draw(sf::RenderWindow& window) {
	level.draw(window);

	player1.draw(window);
	player2.draw(window);

	int middle = 160 - time_text.getLocalBounds().width / 2;
	time_text.setPosition(middle, 16);
	window.draw(time_text);

	window.draw(p1_alias_text);
	window.draw(p2_alias_text);
	
	p1_health_bar.draw(window, player1.getHealth());
	p2_health_bar.draw(window, player2.getHealth());
}

std::vector<int> fr::Match::getControls(std::string section) {
	std::vector<int> controls = {
		config.getInt(section, "left", -1),
		config.getInt(section, "right", -1),
		config.getInt(section, "up", -1),
		config.getInt(section, "down", -1),
		config.getInt(section, "a", -1),
		config.getInt(section, "b", -1),
		config.getInt(section, "x", -1),
		config.getInt(section, "y", -1),
		config.getInt(section, "l", -1),
		config.getInt(section, "r", -1),
		config.getInt(section, "pause", -1),
	};
	
	return controls;
}
