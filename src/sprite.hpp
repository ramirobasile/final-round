#pragma once

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

namespace fr {

class Sprite {
	public:
	Sprite() {};
	Sprite(sf::Texture r_spritesheet, sf::Texture l_spritesheet,
			Animations animations);

	void update(Punch& punch, Punch prev_punch,
			Dodge& dodge, Dodge prev_dodge, float dt);
	void draw(sf::RenderWindow &window, Movement movement, Guard guard, 
			Punch punch, Dodge dodge, Stun stun, sf::FloatRect relative_to, 
			Direction direction);
	
	private:
	static constexpr float FPS = 16;
	static constexpr int SIZE = 128;
	
	Animation getCurrentAnimation(Movement movement, Guard guard, 
			Punch punch, Dodge dodge, Stun stun) const;
	
	sf::Texture r_spritesheet;
	sf::Texture l_spritesheet;
	Animations animations;
	sf::Sprite sprite;
	float progress = 0;
};

} // namespace fr
