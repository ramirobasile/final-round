#include "match.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "character.hpp"
#include "config.hpp"
#include "direction.hpp"
#include "dodge.hpp"
#include "health_bar.hpp"
#include "input.hpp"
#include "level.hpp"
#include "player.hpp"
#include "sounds.hpp"
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
	
	step.loadFromFile(prefix + "step.ogg");
	dodge.loadFromFile(prefix + "dodge.ogg");
	jab.loadFromFile(prefix + "jab.ogg");
	power.loadFromFile(prefix + "power.ogg");
	block.loadFromFile(prefix + "block.ogg");
	hit.loadFromFile(prefix + "hit.ogg");
	ko.loadFromFile(prefix + "ko.ogg");
	ui_move.loadFromFile(prefix + "ui_move.ogg");
	
	// Level
	sf::FloatRect left(8, 112, 8, 64);
	sf::FloatRect right(320 - 16, 112, 8, 64);

	level = Level(left, right, ring);

	// Player 1
	Character character = Character(sf::Sound(jab), sf::Sound(power));
	
	Device device = (Device)config.getInt("player1_controls", "device", 0);

	sf::Texture r_spritesheet, l_spritesheet;
	r_spritesheet.loadFromFile(prefix + "red_right_spritesheet.png");
	l_spritesheet.loadFromFile(prefix + "red_left_spritesheet.png");
	
	Sounds sounds = {
		sf::Sound(step),
		sf::Sound(dodge), 
		sf::Sound(block), 
		sf::Sound(hit),
		sf::Sound(ko), 
		sf::Sound(ui_move)
	};

	player1 = Player(Direction::right, device, getControls("player1_controls"), 
			character.stats, character.punches, character.dodges, r_spritesheet,
			l_spritesheet, character.animations, sounds, 0);
			
	player1.position = sf::Vector2f(16 + 64, 112);
			
	// Player 2
	character = Character(sf::Sound(jab), sf::Sound(power));
	device = (Device)config.getInt("player2_controls", "device", 0);

	r_spritesheet.loadFromFile(prefix + "blue_right_spritesheet.png");
	l_spritesheet.loadFromFile(prefix + "blue_left_spritesheet.png");

	player2 = Player(Direction::left, device, getControls("player2_controls"), 
			character.stats, character.punches, character.dodges, r_spritesheet,
			l_spritesheet, character.animations, sounds, 1);
			
	player2.position = sf::Vector2f(320 - 16 - 64 * 2, 112);

	// UI elements
	// TODO Make values relative to screen size
	time_text = sf::Text("", regular, 8);
	
	win_text = sf::Text("", regular, 8);
	
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
	// Don't update match when it's done
	if (isDone())
		return;
		
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
	if (player1.punch.canInterrupt() && clearbox.intersects(player2.getBounds()))
		player1.punch.interrupt();

	clearbox = player2.punch.getClearbox(player2.getBounds(), player2.getDirection());
	if (player2.punch.canInterrupt() && clearbox.intersects(player1.getBounds()))
		player2.punch.interrupt();
		
	// End on first KO
	if (player1.isKO()) {
		done = true;
		win_text.setString("PLAYER 2 WINS");
	} else if (player2.isKO()) {
		done = true;
		win_text.setString("PLAYER 1 WINS");
	}
	win_text.setPosition(160 - win_text.getLocalBounds().width / 2, 
			120 - win_text.getLocalBounds().height / 2);

	// Round timer
	if (round_time > 0)
		round_time -= dt;

	std::string minutes = std::to_string((int)round_time / 60);
	std::string seconds = fillString(std::to_string((int)round_time % 60), 2, '0');
	time_text.setString(minutes + ":" + seconds);
	time_text.setPosition(160 - time_text.getLocalBounds().width / 2, 16);
}

void fr::Match::draw(sf::RenderWindow& window, float dt) {
	if (isDone()) {
		window.draw(win_text);
	} else {
		level.draw(window);

		player1.draw(window);
		player2.draw(window);

		window.draw(time_text);

		window.draw(p1_alias_text);
		window.draw(p2_alias_text);
		
		p1_health_bar.draw(window, player1.getHealth());
		p2_health_bar.draw(window, player2.getHealth());
		
		// Shake screen when stunned
		if (player1.getStunTime() > MIN_SHAKE_STUN
			|| player2.getStunTime() > MIN_SHAKE_STUN) {
			sf::View view = window.getView();
			
			shake += 2 * PI * SHAKE_SPEED * dt;
			int direction = sign(std::sin(shake));
			view.move(direction, 0);
			
			window.setView(view);
		}
	}
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

bool fr::Match::isDone() const {
	return done;
}
