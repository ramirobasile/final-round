#include "sprite.hpp"

#include <vector>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "state.hpp"

fr::Animation::Animation(sf::Vector2i size, int top, int frames, bool loops)
		: size(size), top(top), frames(frames), loops(loops) {
}

sf::IntRect fr::Animation::getSubrect() const {
	return sf::IntRect(size.x * frame, top, size.x, size.y);
}

void fr::Animation::nextFrame() {
	if (frame < frames - 1)
		frame++;
	else if (loops)
		frame = 0;
}

fr::Sprite::Sprite() {} // Empty constructor

fr::Sprite::Sprite(sf::Texture spritesheet, std::vector<Animation> animations, 
		float fps) 
		: spritesheet(spritesheet), animations(animations), fps(fps) {
}

void fr::Sprite::update(fr::State state, fr::State last_state, float dt) {
	progress += dt;

	if (progress > (1 / fps)) {
		getAnimation().nextFrame();
		progress = 0;
	}

	// TODO Figure out which animation
	switch (state.movement) {
		case Movement::idle:
			if (state.guard_high)
				animation = Animations::idle_head;
			else if (state.guard_low)
				animation = Animations::idle_body;
			else
				animation = Animations::idle;
			break;

		case Movement::walk_l:
			animation = Animations::walk_l;
			break;

		case Movement::walk_r:
			animation = Animations::walk_l; // Temp
			break;
	}

	if (state.isPunching())
		animation = Animations::jab; // Temp
}

void fr::Sprite::draw(sf::RenderWindow &window, sf::IntRect relative_to) {
	sf::Sprite sprite(spritesheet, getAnimation().getSubrect());
	int left = relative_to.left + (relative_to.width - getAnimation().size.x) / 2;
	int top = relative_to.top + (relative_to.height - getAnimation().size.y)/ 2;
	sprite.setPosition(left, top);
	window.draw(sprite);
}

fr::Animation &fr::Sprite::getAnimation() {
	return animations[(int)animation];
}
