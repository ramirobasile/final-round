#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"
#include "animations.hpp"

namespace fr {

class Punch {
	public:
	Punch();
	Punch(Controls control, Actions action, Controls mod, float min_held,
			float max_held, int damage, int perma_damage, int block_damage,
			int self_damage, float interrupt_end, float hitbox_begin,
			float hitbox_end, float recovery_end, bool needs_clear, 
			sf::FloatRect hitbox, Animations animation);

	void start();
	void interrupt();
	void end();
	bool canInterrupt() const;
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;
	sf::FloatRect getHitbox(sf::FloatRect relative_to, int direction) const;

	float progress = -1;
	Controls control;
	Actions action;
	Controls mod;
	float min_held;
	float max_held;
	int damage;
	int perma_damage;
	int block_damage;
	int self_damage;
	bool needs_clear;
	Animations animation;

	private:
	float interrupt_end;
	float hitbox_begin;
	float hitbox_end;
	float recovery_end;
	sf::FloatRect hitbox;
};

} // namespace fr
