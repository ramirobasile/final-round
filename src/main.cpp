#include "main.hpp"

#include <iostream>
#include <fstream>

#include <toml++/toml.h>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "match.hpp"

sf::Clock global_clock; // This is mainly used to make logs more readable
sf::Clock dt_clock;
int fr::global_time;
float fr::dt;
toml::table fr::config;

int main() {
    try {
        fr::config = toml::parse_file("fr.toml");
    }
    catch (const toml::parse_error& err) {
        std::cerr << "Parsing failed:\n" << err << "\n";
    }

	int width = fr::config["video"]["width"].value_or(640);
	int height = fr::config["video"]["height"].value_or(480);
	int fps = fr::config["video"]["fps"].value_or(60);
    sf::RenderWindow window (sf::VideoMode(width, height), "Final Round");
	window.setFramerateLimit(fps); // Otherwise SFML has stupid high CPU usage

	fr::Match match;

    while (window.isOpen()) {
    	fr::global_time = global_clock.getElapsedTime().asMilliseconds();
		fr::dt = dt_clock.restart().asSeconds();

		// Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		// Update calls
        match.update();

		// Draw calls
        window.clear();
        match.draw(window);
        window.display();
    }

    return 0;
}
