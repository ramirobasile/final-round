#include "sprite.hpp"

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "state.hpp"

fr::Animation::Animation(int frames, bool loops,
		std::vector<fr::Animations> continues)
		: frames(frames), loops(loops), continues(continues) {
}

void fr::Animation::nextFrame() {
	if (frame < frames - 1)
		frame++;
	else if (loops)
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

	Animations new_animation;
	
	switch (state.movement) {
		case Movements::idle:
			if (state.guard == Guards::head)
				new_animation = Animations::idle_head;
			else if (state.guard == Guards::body)
				new_animation = Animations::idle_body;
			else
				new_animation = Animations::idle;
			break;
		case Movements::walk_l:
		case Movements::walk_r:
			if (state.guard== Guards::head)
				new_animation = Animations::walk_head;
			else if (state.guard == Guards::body)
				new_animation = Animations::walk_body;
			else
				new_animation = Animations::walk;
			break;
	}
	
	switch (state.dodge) {
		case Dodges::pull:
			new_animation = Animations::pull;
			break;
		case Dodges::slip:
			new_animation = Animations::slip;
			break;
		case Dodges::duck:
			new_animation = Animations::duck;
			break;
	}
	
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

	if (animation != new_animation) {
		Animations old_animation = animation;
		int old_frame = getAnimation().frame;
		animation = new_animation;

		bool cont = false;
		for (int i = 0; i < getAnimation().continues.size() && !cont; ++i)
			cont = old_animation == getAnimation().continues[i];

		if (cont) {
			getAnimation().frame = old_frame;
		} else {
			progress = 0;
			getAnimation().frame = 0;
		}
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
