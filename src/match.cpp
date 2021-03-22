#include "match.hpp"

#include <vector>
#include <string>

#include <toml++/toml.h>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
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
		std::string name = "player" + std::to_string(i + 1);
		Device device = (Device)config["controls"][name]["device"].value_or(1);
		std::vector<int> controls = {
			config["controls"][name]["backwards"].value_or(0),
			config["controls"][name]["forwards"].value_or(0),
			config["controls"][name]["up"].value_or(0),
			config["controls"][name]["down"].value_or(0),
			config["controls"][name]["jab"].value_or(0),
			config["controls"][name]["cross"].value_or(0),
			config["controls"][name]["hook"].value_or(0),
			config["controls"][name]["upper"].value_or(0),
			config["controls"][name]["dodge"].value_or(0),
			config["controls"][name]["start"].value_or(0),
		};
		
		sf::Texture spritesheet;
		spritesheet.loadFromFile("assets/placeholder.png");
		std::vector<fr::Animation> animations;

		// Load punches from default_punches.toml
		toml::table table;
		try {
        	table = toml::parse_file("default_punches.toml");
		}
		catch (const toml::parse_error& err) {
		    std::cerr << "Parsing failed:\n" << err << "\n";
		}
		toml::array arr = *table.get_as<toml::array>("punches");
		std::vector<fr::Punch> punches = getPunches(arr);

		fr::Stats stats(0, 0, 0);
		
		players.push_back(Player(i, direction, device, controls, 
				spawn_points[i], spritesheet, animations, punches, stats));
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
