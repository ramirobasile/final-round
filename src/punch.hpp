#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

namespace fr {

class Punch {
	public:
	Punch();
	Punch(Control control, Action action, Control mod, float min_held,
			float max_held, int damage, int perma_damage, int block_damage,
			int self_damage, float interrupt_end, float hitbox_begin,
			float hitbox_end, float recovery_end, bool needs_clear, 
			sf::FloatRect hitbox);

	void start();
	void interrupt();
	void end();
	bool canInterrupt() const;
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;
	sf::FloatRect getHitbox(sf::FloatRect relative_to, int direction) const;

	float progress = -1;
	Control control;
	Action action;
	Control mod;
	float min_held;
	float max_held;
	int damage;
	int perma_damage;
	int block_damage;
	int self_damage;
	bool needs_clear;

	private:
	float interrupt_end;
	float hitbox_begin;
	float hitbox_end;
	float recovery_end;
	sf::FloatRect hitbox;
};

extern std::vector<Punch> default_punches;

} // namespace fr
