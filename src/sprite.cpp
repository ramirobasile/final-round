#include "sprite.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "animation.hpp"
#include "direction.hpp"
#include "dodge.hpp"
#include "guard.hpp"
#include "movement.hpp"
#include "punch.hpp"
#include "stun.hpp"

fr::Sprite::Sprite(sf::Texture r_spritesheet, sf::Texture l_spritesheet,
		std::vector<Animation> animations)
		: r_spritesheet(r_spritesheet), l_spritesheet(l_spritesheet),
		animations(animations) {
}

void fr::Sprite::update(fr::Punch punch, fr::Punch prev_punch, fr::Dodge dodge,
		fr::Dodge prev_dodge, float dt) {
	progress += dt;

	if (progress > (1 / FPS)) {
		progress = 0;
		
		if (punch.isDone() && dodge.isDone()) {
			getAnimation(Animation::Name::walk).nextFrame();
			getAnimation(Animation::Name::walk_guard_head).nextFrame();
			getAnimation(Animation::Name::walk_guard_body).nextFrame();
		}
		
		if (!dodge.isDone())
			getAnimation(dodge.getAnimation()).nextFrame();
			
		if (!punch.isDone())
			getAnimation(punch.getAnimation()).nextFrame();
	}
		
	if (prev_dodge.isDone() && !dodge.isDone()) {
		getAnimation(dodge.getAnimation()).reset();
		progress = 0;
	}
	
	if (prev_punch.isDone() && !punch.isDone()) {
		getAnimation(punch.getAnimation()).reset();
		progress = 0;
	}
}

void fr::Sprite::draw(sf::RenderWindow &window, fr::Movement movement, 
		fr::Guard guard, fr::Punch punch, fr::Dodge dodge, fr::Stun stun,
		sf::FloatRect relative_to, fr::Direction direction) {
	Animation::Name animation = getCurrentAnimation(movement, guard, punch, 
			dodge, stun);
	
	sf::IntRect subrect(SIZE * getAnimation(animation).getFrame(),
			SIZE * (int)animation, SIZE, SIZE);
	sprite.setTextureRect(subrect);

	if (direction == Direction::right)
		sprite.setTexture(r_spritesheet);
	else
		sprite.setTexture(l_spritesheet);

	int left = relative_to.left + (relative_to.width - SIZE) / 2;
	int top = relative_to.top + (relative_to.height - SIZE) / 2;
	sprite.setPosition(left, top);

	window.draw(sprite);
}

// Yeah, I know...
fr::Animation::Name fr::Sprite::getCurrentAnimation(fr::Movement movement, 
		fr::Guard guard, fr::Punch punch, fr::Dodge dodge, 
		fr::Stun stun) const  {
	Animation::Name animation = Animation::Name::idle;
	
	switch (movement) {
		case Movement::idle:
			switch (guard) {
				case Guard::head:
					animation = Animation::Name::idle_guard_head;
					break;
				case Guard::body:
					animation = Animation::Name::idle_guard_body;
					break;
			}
			break;

		case Movement::walk_b:
		case Movement::walk_f:
			switch (guard) {
				case Guard::head:
					animation = Animation::Name::walk_guard_head;
					break;
				case Guard::body:
					animation = Animation::Name::walk_guard_body;
					break;
				default:
					animation = Animation::Name::walk;
					break;
			}
			break;

		case Movement::stun:
			switch (stun) {
				case Stun::head:
					animation = Animation::Name::hit_head;
					break;
				case Stun::body:
					animation = Animation::Name::hit_body;
					break;
				case Stun::block_head:
					animation = Animation::Name::idle_guard_head;
					break;
				case Stun::block_body:
					animation = Animation::Name::idle_guard_body;
					break;
			}
			break;
	}
	
	if (!dodge.isDone())
		animation = dodge.getAnimation();
		
	if (!punch.isDone())
		animation = punch.getAnimation();
	
	return animation;
}

fr::Animation& fr::Sprite::getAnimation(fr::Animation::Name animation) {
	return animations[(int)animation];
}
