#include "sprite.hpp"

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "state.hpp"

fr::Animation::Animation(int frames, bool loops, bool reusmes) 
		: frames(frames), loops(loops), resumes(resumes) {
}

void fr::Animation::nextFrame() {
	if (frame < frames - 1)
		frame++;
	else if (loops)
		frame = 0;
}

void fr::Animation::reset() {
	if (!resumes)
		frame = 0;
}

fr::Sprite::Sprite() {} // Empty constructor

fr::Sprite::Sprite(sf::Texture l_spritesheet, sf::Texture r_spritesheet, 
		std::vector<Animation> animations, sf::Vector2i size, float fps) 
		: l_spritesheet(l_spritesheet), r_spritesheet(r_spritesheet), 
		animations(animations), size(size), fps(fps) {
}

void fr::Sprite::update(fr::State state, fr::State last_state, float dt) {
	progress += dt;

	if (progress > (1 / fps)) {
		getAnimation().nextFrame();
		progress = 0;
	}

	// TODO
	Animations new_animation;
	switch (state.movement) {
		case Movement::idle:
			if (state.guard_high)
				new_animation = Animations::idle_head;
			else if (state.guard_low)
				new_animation = Animations::idle_body;
			else
				new_animation = Animations::idle;
			break;
		case Movement::walk_l:
			if (state.guard_high)
				new_animation = Animations::walk_l_head;
			else if (state.guard_low)
				new_animation = Animations::walk_l_body;
			else
				new_animation = Animations::walk_l;
			break;
		case Movement::walk_r:
			if (state.guard_high)
				new_animation = Animations::walk_r_head;
			else if (state.guard_low)
				new_animation = Animations::walk_r_body;
			else
				new_animation = Animations::walk_r;
			break;
	}

	if (state.isPunching()) {
		switch (state.punch) {
			case Punches::jab_body:
				new_animation = Animations::jab_body;
				break;
			case Punches::jab_head:
				new_animation = Animations::jab_head;
				break;
			case Punches::cross_body:
				new_animation = Animations::cross_body;
				break;
			case Punches::cross_head:
				new_animation = Animations::cross_head;
				break;
			case Punches::hook_body:
				new_animation = Animations::hook_body;
				break;
			case Punches::hook_head:
				new_animation = Animations::hook_head;
				break;
			case Punches::upper_body:
				new_animation = Animations::upper_body;
				break;
			case Punches::upper_head:
				new_animation = Animations::upper_head;
				break;
		}
	}

	if (animation != new_animation) {
		progress = 0;
		getAnimation().reset();
		animation = new_animation;
	}
}

void fr::Sprite::draw(sf::RenderWindow &window, sf::FloatRect relative_to,
		int direction) {
	sf::IntRect subrect(size.x * getAnimation().frame, size.y * (int)animation,
			size.x, size.y);

	sf::Sprite sprite;
	if (direction == 1)
		sprite = sf::Sprite(l_spritesheet, subrect);
	else
		sprite = sf::Sprite(r_spritesheet, subrect);

	int left = relative_to.left + (relative_to.width - size.x) / 2;
	int top = relative_to.top + (relative_to.height - size.y)/ 2;
	sprite.setPosition(left, top);

	window.draw(sprite);
}

fr::Animation &fr::Sprite::getAnimation() {
	return animations[(int)animation];
}
