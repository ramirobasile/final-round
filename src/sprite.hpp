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
	Animation(sf::Vector2i size, int top, int frames, bool loops);
	
	sf::IntRect getSubrect() const;
	void nextFrame();
	
	sf::Vector2i size;
	
	private:
	int frame = 0;
	int top;
	int frames;
	bool loops;
};

class Sprite {
	public:
	Sprite();
	Sprite(sf::Texture spritesheet, std::vector<Animation> animations, float fps);

	void update(State state, State last_state, float dt);
	void draw(sf::RenderWindow &window, sf::IntRect relative_to);
	Animation &getAnimation();
	
	private:
	Animations animation = Animations::idle;
	sf::Texture spritesheet;
	float fps;
	std::vector<Animation> animations;
	float progress = 0;
};

} // namespace fr
