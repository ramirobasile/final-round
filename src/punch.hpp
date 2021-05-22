#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "animation.hpp"
#include "direction.hpp"
#include "input.hpp"

namespace fr {

struct Hit {
	int damage = -1;
	int perma_damage = -1;
	int block_damage = -1;
	float hit_stun = -1;
	float block_stun = -1;
	bool head = false;
};

class Punch {
	public:
	Punch() {}; // Empty constructor
	Punch(Control control, Control mod, Hit hit, int cost, float interrupt_end,
			float hitbox_begin, float hitbox_end, float recovery_end, 
			sf::FloatRect hitbox, sf::FloatRect clearbox, 
			Animation::Name animation);

	void update(float dt);
	void reset();
	void end();
	void interrupt();
	bool canInterrupt() const;
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;
	Control getControl() const;
	Control getMod() const;
	Hit getHit() const;
	int getCost() const;
	sf::FloatRect getHitbox(sf::FloatRect relative_to,
			Direction direction) const;
	sf::FloatRect getClearbox(sf::FloatRect relative_to,
			Direction direction) const;
	Animation::Name getAnimation() const;

	private:
	static constexpr int BODY_OFFSET = 14;
	static constexpr float FEINT_END = 0.1f;

	Control control = Control::none;
	Control mod = Control::none;
	Hit hit;
	int cost = -1;
	float interrupt_end = -1;
	float hitbox_begin = -1;
	float hitbox_end = -1;
	float recovery_end = -1;
	sf::FloatRect hitbox;
	sf::FloatRect clearbox;
	float progress = -1;
	Animation::Name animation = Animation::Name::idle;
};

} // namespace fr
