#include "sprite.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "direction.hpp"
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
	
	if (!state.dodge.isDone())
		new_animation = state.dodge.animation;
		
	if (!state.punch.isDone())
		new_animation = state.punch.animation;

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
		fr::Direction direction) {
	sf::IntRect subrect(size.x * getAnimation().frame, size.y * (int)animation,
			size.x, size.y);

	sf::Sprite sprite;
	if (direction == Direction::right)
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
