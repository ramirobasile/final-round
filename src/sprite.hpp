#pragma once

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "animations.hpp"
#include "state.hpp"
#include "direction.hpp"

namespace fr {

class Animation {
	public:
	Animation(int frames, bool loops, std::vector<Animations> continues);
	
	void nextFrame();
	
	int frame = 0;
	std::vector<Animations> continues;
	
	private:
	int frames;
	bool loops;
};

class Sprite {
	public:
	Sprite();
	Sprite(sf::Texture l_spritesheet, sf::Texture r_spritesheet, 
			std::vector<Animation> animations, sf::Vector2i size, float fps);

	void update(State state, State last_state, float dt);
	void draw(sf::RenderWindow &window, sf::FloatRect relative_to, 
			Direction direction);
	Animation &getAnimation();
	
	private:
	sf::Texture l_spritesheet;
	sf::Texture r_spritesheet;
	std::vector<Animation> animations;
	sf::Vector2i size;
	float fps;
	Animations animation = Animations::idle;
	float progress = 0;
};

} // namespace fr
