#pragma once

#include <vector>

#include "SFML/Audio.hpp"
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
	bool can_ko = false;
};

class Punch {
	public:
	Punch() {}; // Empty constructor
	
	// Head punch constructor
	Punch(Control control, Hit hit, int cost, float hit_range, float stuff_range, 
			float interrupt_end, float hitbox_begin, float hitbox_end, 
			float recovery_end, Animation animation, sf::Sound sound); 
			
	// Body punch constructor
	Punch(Punch head_punch, Hit hit, float range, float min_distance, 
			Animation animation); 

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
	bool hits(sf::FloatRect hurtbox, sf::FloatRect relative_to, 
			Direction direction) const;
	bool getsStuffed(sf::FloatRect hurtbox, sf::FloatRect relative_to, 
			Direction direction) const;
	
	Animation animation;
	sf::Sound sound;

	private:
	static constexpr Control BODY_MOD = Control::body;

	Control control = Control::none;
	Control mod = Control::none;
	Hit hit;
	int cost = -1;
	float hit_range = -1;
	float stuff_range = -1;
	float interrupt_end = -1;
	float active_begin = -1;
	float active_end = -1;
	float recovery_end = -1;
	float progress = -1;
};

} // namespace fr
