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
		fr::Guard guard, fr::Punch punch, fr::Dodge dodge, 
		sf::FloatRect relative_to, fr::Direction direction) {
	Animation animation = getMovementAnim(movement, guard);
	
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

fr::Animation fr::Sprite::getMovementAnim(fr::Movement movement, 
		fr::Guard guard) const  {
	Animation animation = movement_anims[(int)MovementAnims::idle];
	
	switch (movement) {
		case Movement::idle:
			if (guard == Guard::head)
				animation = movement_anims[(int)MovementAnims::idle_head];
			else if (guard == Guard::body)
				animation = movement_anims[(int)MovementAnims::idle_body];
			else
				animation = movement_anims[(int)MovementAnims::idle];
			break;

		case Movement::walk_b:
		case Movement::walk_f:
			if (guard== Guard::head)
				animation = movement_anims[(int)MovementAnims::walk_head];
			else if (guard == Guard::body)
				animation = movement_anims[(int)MovementAnims::walk_body];
			else
				animation = movement_anims[(int)MovementAnims::walk];
			break;

		case Movement::stun:
			break;
	}
	
	return animation;
}
