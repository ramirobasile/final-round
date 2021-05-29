#include "sprite.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "animation.hpp"
#include "animations.hpp"
#include "direction.hpp"
#include "dodge.hpp"
#include "guard.hpp"
#include "movement.hpp"
#include "punch.hpp"
#include "stun.hpp"

fr::Sprite::Sprite(sf::Texture r_spritesheet, sf::Texture l_spritesheet,
		Animations animations)
		: r_spritesheet(r_spritesheet), l_spritesheet(l_spritesheet),
		animations(animations) {
}

// HACK Ugly (maybe) avoidable ref pass
void fr::Sprite::update(fr::Punch& punch, fr::Punch prev_punch, fr::Dodge& dodge,
		fr::Dodge prev_dodge, float dt) {
	progress += dt;

	if (progress > (1 / FPS)) {
		progress = 0;
		
		if (punch.isDone() && dodge.isDone()) {
			animations.walk.nextFrame();
			animations.walk_guard_head.nextFrame();
			animations.walk_guard_body.nextFrame();
		}
		
		if (!dodge.isDone())
			dodge.animation.nextFrame();
			
		if (!punch.isDone())
			punch.animation.nextFrame();
	}
		
	if (prev_dodge.isDone() && !dodge.isDone()) {
		dodge.animation.reset();
		progress = 0;
	}
	
	if (prev_punch.isDone() && !punch.isDone()) {
		punch.animation.reset();
		progress = 0;
	}
}

void fr::Sprite::draw(sf::RenderWindow &window, fr::Movement movement, 
		fr::Guard guard, fr::Punch punch, fr::Dodge dodge, fr::Stun stun,
		sf::FloatRect relative_to, fr::Direction direction) {
	Animation animation = getCurrentAnimation(movement, guard, punch, 
			dodge, stun);
	
	sf::IntRect subrect(SIZE * animation.getFrame(), 
			SIZE * animation.getSheetY(), SIZE, SIZE);
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
fr::Animation fr::Sprite::getCurrentAnimation(fr::Movement movement, 
		fr::Guard guard, fr::Punch punch, fr::Dodge dodge, 
		fr::Stun stun) const  {
	Animation animation = animations.idle;
	
	switch (movement) {
		case Movement::idle:
			switch (guard) {
				case Guard::head:
					animation = animations.idle_guard_head;
					break;
				case Guard::body:
					animation = animations.idle_guard_body;
					break;
			}
			break;

		case Movement::walk_b:
		case Movement::walk_f:
			switch (guard) {
				case Guard::head:
					animation = animations.walk_guard_head;
					break;
				case Guard::body:
					animation = animations.walk_guard_body;
					break;
				default:
					animation = animations.walk;
					break;
			}
			break;

		case Movement::stun:
			switch (stun) {
				case Stun::head:
					animation = animations.hit_head;
					break;
				case Stun::body:
					animation = animations.hit_body;
					break;
				case Stun::block_head:
					animation = animations.idle_guard_head;
					break;
				case Stun::block_body:
					animation = animations.idle_guard_body;
					break;
			}
			break;
	}
	
	if (!dodge.isDone())
		animation = dodge.animation;
		
	if (!punch.isDone())
		animation = punch.animation;
	
	return animation;
}
