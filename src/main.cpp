#include "main.hpp"

#include <iostream>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "match.hpp"

int fr::global_time;
float fr::dt;
fr::ConfigFile fr::config;

int main() {
	fr::config.parse(std::ifstream("fr.cfg"));

	int scale = fr::config.getFloat("video", "scale", 1.f);
	int width = fr::config.getInt("video", "width", 320) * scale;
	int height = fr::config.getInt("video", "height", 240) * scale;
    sf::RenderWindow window (sf::VideoMode(width, height), "Final Round");
	sf::View view;
	view.reset(sf::FloatRect(0, 0, width, height));
	view.setViewport(sf::FloatRect(0.f, 0.f, scale, scale));
    window.setView(view);
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
