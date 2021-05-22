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
		std::vector<Animation> movement_anims)
		: r_spritesheet(r_spritesheet), l_spritesheet(l_spritesheet),
		movement_anims(movement_anims) {
}

void fr::Sprite::update(fr::Punch punch, fr::Punch prev_punch, fr::Dodge dodge,
		fr::Dodge prev_dodge, float dt) {
	progress += dt;

	if (progress > (1 / FPS)) {
		progress = 0;
		
		if (punch.isDone() && dodge.isDone()) {
			for (int i = 0; i< movement_anims.size(); ++i)
				movement_anims[i].nextFrame();
		}
		
		if (!dodge.isDone())
			dodge_animation.nextFrame();
			
		if (!punch.isDone())
			punch_animation.nextFrame();
	}
		
	if (prev_dodge.isDone() && !dodge.isDone()) {
		dodge_animation = dodge.animation;
		progress = 0;
	}
	
	if (prev_punch.isDone() && !punch.isDone()) {
		punch_animation = punch.animation;
		progress = 0;
	}
}

void fr::Sprite::draw(sf::RenderWindow &window, fr::Movement movement, 
		fr::Guard guard, fr::Punch punch, fr::Dodge dodge, fr::Stun stun,
		sf::FloatRect relative_to, fr::Direction direction) {
	Animation animation = getMovementAnim(movement, guard, stun);
	
	if (!dodge.isDone())
		animation = dodge_animation;
		
	if (!punch.isDone())
		animation = punch_animation;
	
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
fr::Animation fr::Sprite::getMovementAnim(fr::Movement movement, 
		fr::Guard guard, fr::Stun stun) const  {
	Animation animation = movement_anims[(int)MovementAnims::idle];
	
	switch (movement) {
		case Movement::idle:
			switch (guard) {
				case Guard::head:
					animation = movement_anims[(int)MovementAnims::idle_guard_head];
					break;
				case Guard::body:
					animation = movement_anims[(int)MovementAnims::idle_guard_body];
					break;
			}
			break;

		case Movement::walk_b:
		case Movement::walk_f:
			switch (guard) {
				case Guard::head:
					animation = movement_anims[(int)MovementAnims::walk_guard_head];
					break;
				case Guard::body:
					animation = movement_anims[(int)MovementAnims::walk_guard_body];
					break;
				default:
					animation = movement_anims[(int)MovementAnims::walk];
					break;
			}
			break;

		case Movement::stun:
			switch (stun) {
				case Stun::head:
					animation = movement_anims[(int)MovementAnims::hit_head];
					break;
				case Stun::body:
					animation = movement_anims[(int)MovementAnims::hit_body];
					break;
				case Stun::block_head:
					animation = movement_anims[(int)MovementAnims::idle_guard_head];
					break;
				case Stun::block_body:
					animation = movement_anims[(int)MovementAnims::idle_guard_body];
					break;
			}
			break;
	}
	
	return animation;
}
