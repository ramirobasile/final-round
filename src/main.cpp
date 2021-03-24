#include "main.hpp"

#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "inih/cpp/INIReader.h"
#include "match.hpp"

int fr::global_time;
float fr::dt;
INIReader fr::config("fr.config");

int main() {
    if (fr::config.ParseError() < 0) {
        std::cout << "Error loading 'fr.config'" << std::endl;
        return 1;
    }

	int width = fr::config.GetInteger("video", "width", 640);
	int height = fr::config.GetInteger("video", "height", 480);
    sf::RenderWindow window (sf::VideoMode(width, height), "Final Round");
	window.setFramerateLimit(fr::FPS); // Otherwise SFML has stupid high CPU usage


	sf::Clock global_clock;
	sf::Clock dt_clock;
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
