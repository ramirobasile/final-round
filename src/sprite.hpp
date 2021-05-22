#pragma once

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

namespace fr {

enum class MovementAnims {
	idle,
	idle_guard_head,
	idle_guard_body,
	walk,
	walk_guard_head,
	walk_guard_body,
	hit_head,
	hit_body,
	ko,
};

class Sprite {
	public:
	Sprite() {};
	Sprite(sf::Texture r_spritesheet, sf::Texture l_spritesheet,
			std::vector<Animation> movement_anims);

	void update(Punch punch, Punch prev_punch,
			Dodge dodge, Dodge prev_dodge, float dt);
	void draw(sf::RenderWindow &window, Movement movement, Guard guard, 
			Punch punch, Dodge dodge, Stun stun, sf::FloatRect relative_to, 
			Direction direction);
	
	private:
	static constexpr float FPS = 16;
	static constexpr int SIZE = 128;
	
	Animation getMovementAnim(Movement movement, Guard guard, Stun stun) const;
	
	sf::Texture r_spritesheet;
	sf::Texture l_spritesheet;
	std::vector<Animation> movement_anims;
	sf::Sprite sprite;
	float progress = 0;
	Animation dodge_animation;
	Animation punch_animation;
};

} // namespace fr
