#include <iostream>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "config.hpp"
#include "match.hpp"

const float FPS = 60;

int main() {
	fr::ConfigFile config;
	config.parse(std::ifstream("fr.cfg"));

	int scale = config.getFloat("video", "scale", 1.f);
	int width = config.getInt("video", "width", 640) * scale;
	int height = config.getInt("video", "height", 480) * scale;
    sf::RenderWindow window (sf::VideoMode(width, height), "Final Round");
	sf::View view;
	view.reset(sf::FloatRect(0, 0, width, height));
	view.setViewport(sf::FloatRect(0.f, 0.f, scale, scale));
    window.setView(view);
	window.setFramerateLimit(FPS); // Otherwise SFML has stupid high CPU usage

	sf::Clock global_clock;
	int global_time;
	sf::Clock dt_clock;
	float dt;
	fr::Match match(config);

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

        if (config.getBool("debug", "log_state", false))
        	match.logState(global_time);

        if (config.getBool("debug", "log_inputs", false))
        	match.logInputs(global_time);

		// Draw calls
        window.clear();

        match.draw(window);

        if (config.getBool("debug", "draw_geometry", false))
        	match.drawDebugGeometry(window);

        if (config.getBool("debug", "draw_hurtboxes", false))
        	match.drawDebugHurtboxes(window);

        if (config.getBool("debug", "draw_hitboxes", false))
        	match.drawDebugHitboxes(window);

        if (config.getBool("debug", "draw_info", false))
        	match.drawDebugInfo(window);

        window.display();
    }

    return 0;
}
