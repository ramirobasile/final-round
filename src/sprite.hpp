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
	walk_l,
	walk_r,
	walk_l_head,
	walk_r_head,
	walk_l_body,
	walk_r_body,
	jab,
	body_jab,
	cross,
	body_cross,
	upper,
	body_upper,
	hook,
	body_hook,
	pull,
	weave,
	duck,
	hit_head,
	hit_body,
	ko,
};

class Animation {
	public:
	Animation(int frames, bool loops);
	
	void nextFrame();
	
	int frame = 0;
	
	private:
	int frames;
	bool loops;
};

class Sprite {
	public:
	Sprite();
	Sprite(sf::Texture spritesheet, std::vector<Animation> animations, 
			sf::Vector2i size, float fps);

	void update(State state, State last_state, float dt);
	void draw(sf::RenderWindow &window, sf::IntRect relative_to);
	Animation &getAnimation();
	
	private:
	sf::Texture spritesheet;
	std::vector<Animation> animations;
	sf::Vector2i size;
	float fps;
	Animations animation = Animations::idle;
	float progress = 0;
};

} // namespace fr
