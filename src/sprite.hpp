#pragma once

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "state.hpp"

namespace fr {

enum class Animations {
	idle,
	idle_head,
	idle_body,
	walk,
	walk_head,
	walk_body,
	jab_body,
	jab_head,
	cross_body,
	cross_head,
	hook_body,
	hook_head,
	upper_body,
	upper_head,
	pull,
	slip,
	duck,
	hit_head,
	hit_body,
	ko,
};

class Animation {
	public:
	Animation(int frames, bool loops, bool resumes);
	
	void nextFrame();
	void reset();
	
	int frame = 0;
	
	private:
	int frames;
	bool loops;
	bool resumes;
};

class Sprite {
	public:
	Sprite();
	Sprite(sf::Texture l_spritesheet, sf::Texture r_spritesheet, 
			std::vector<Animation> animations, sf::Vector2i size, float fps);

	void update(State state, State last_state, float dt);
	void draw(sf::RenderWindow &window, sf::FloatRect relative_to, int direction);
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
