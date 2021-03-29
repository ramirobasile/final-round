#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

namespace fr {

class Punch {
	public:
	Punch();
	Punch(Control control, Control mod, float min_held_time, bool lead_handed,
			bool body_shot, int hit_damage, int perm_hit_damage,
			int block_damage, int self_damage, float interrupt_end,
			float hitbox_begin, float hitbox_end, float recovery_end,
			sf::IntRect hitbox, sf::IntRect clearbox);

	float progress = -1;
	Control control;
	Control mod;
	float min_held_time;
	bool lead_handed;
	bool body_shot;
	int hit_damage;
	int perm_hit_damage;
	int block_damage;
	int self_damage;
	float interrupt_end;
	float hitbox_begin;
	float hitbox_end;
	float recovery_end;

	void start();
	void interrupt();
	void end();
	bool interruptible() const;
	bool active() const;
	bool recovering() const;
	bool done() const;
	sf::IntRect getHitbox(sf::Vector2f relative_to, int direction) const;
	sf::IntRect getClearbox(sf::Vector2f relative_to, int direction) const;

	private:
	sf::IntRect hitbox;
	sf::IntRect clearbox;
};

extern std::vector<Punch> default_punches;

} // namespace fr
