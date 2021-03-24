#include "match.hpp"

#include <vector>
#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "inih/cpp/INIReader.h"
#include "main.hpp"
#include "level.hpp"
#include "player.hpp"
#include "input.hpp"
#include "stats.hpp"
#include "utils.hpp"

fr::Match::Match() {
	// Level
	sf::IntRect left(0, 300, 40, 100);
	sf::IntRect right(600, 300, 40, 100);
	level = Level{left, right};
	
	// Players
	std::vector<sf::Vector2f> spawn_points{sf::Vector2f(100, 300), 
			sf::Vector2f(440, 300)};
			
	for (int i = 0; i < 2; ++i) {
		int direction = ((i % 2) - 1) * - 1;
		
		// Load controls from config.toml
		std::string map_name = "player" + std::to_string(i + 1) + "_controls";
		Device device = (Device)config.GetInteger(map_name, "decive", 1);
		std::vector<int> controls = {
			config.GetInteger(map_name, "backwards", -1),
			config.GetInteger(map_name, "forwards", -1),
			config.GetInteger(map_name, "up", -1),
			config.GetInteger(map_name, "down", -1),
			config.GetInteger(map_name, "jab", -1),
			config.GetInteger(map_name, "cross", -1),
			config.GetInteger(map_name, "hook", -1),
			config.GetInteger(map_name, "upper", -1),
			config.GetInteger(map_name, "dodge", -1),
			config.GetInteger(map_name, "start", -1),
		};
		
		sf::Texture spritesheet;
		spritesheet.loadFromFile("assets/placeholder.png");
		std::vector<fr::Animation> animations;
		fr::Stats stats(1, 1, 1);
		
		players.push_back(Player(i, direction, device, controls, 
				spawn_points[i], spritesheet, animations, stats));
	}
}

void fr::Match::update() {
	for (int i = 0; i < players.size(); ++i) {
		std::vector geometry(level.geometry());
		
		// Add every other player's collisions
		for (int j = 0; j < players.size(); ++j) {
			if (i != j)
				geometry.push_back(players[j].bounds);
		}
		
		players[i].update(geometry);
	}
}

void fr::Match::draw(sf::RenderWindow &window) {
	level.draw(window);

	for (int i = 0; i < players.size(); ++i) {
		players[i].draw(window);
	}
}
