#include "main.hpp"

#include <iostream>
#include <fstream>

#include <toml++/toml.h>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "match.hpp"

sf::Clock global_clock; // This is mainly used to make logs more readable
sf::Clock dt_clock;

int main() {
    try {
        config = toml::parse_file("fr.toml");
    }
    catch (const toml::parse_error& err) {
        std::cerr << "Parsing failed:\n" << err << "\n";
    }

	int width = config["video"]["width"].value_or(640);
	int height = config["video"]["height"].value_or(480);
	int fps = config["video"]["fps"].value_or(60);
    sf::RenderWindow window (sf::VideoMode(width, height), "Final Round");
	window.setFramerateLimit(fps); // Otherwise SFML has stupid high CPU usage

    while (window.isOpen()) {
    	global_time = global_clock.getElapsedTime().asMilliseconds();
		dt = dt_clock.restart().asSeconds();

		// Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		// Update calls
        match.update(dt, global_time);

		// Draw calls
        window.clear();
        match.draw(window);
        window.display();
    }

    return 0;
}
