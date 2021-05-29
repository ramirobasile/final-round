#include <fstream>
#include <iostream>

#include <SFML/Audio.hpp>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "match.hpp"

const float FPS = 60;
const int WIDTH = 320;
const int HEIGHT = 240;

int main() {
	fr::ConfigFile config;
	config.parse(std::ifstream("../etc/final-round.cfg"));

	int scale = config.getFloat("video", "scale", 1.f);
	int width = WIDTH * scale;
	int height = HEIGHT * scale;
    sf::RenderWindow window (sf::VideoMode(width, height), "Final Round");
	sf::View view;
	view.reset(sf::FloatRect(0, 0, width, height));
	view.setViewport(sf::FloatRect(0.f, 0.f, scale, scale));
    window.setView(view);
	window.setFramerateLimit(FPS); // Otherwise SFML has stupid high CPU usage

	sf::Clock dt_clock;
	float dt;
	fr::Match match(config);

    while (window.isOpen()) {
		dt = dt_clock.restart().asSeconds();

		// Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		// Update calls
        match.update(dt);

		// Draw calls
        window.clear();

        match.draw(window, dt);

        window.display();
    }

    return 0;
}
