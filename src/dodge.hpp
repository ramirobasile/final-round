#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "animations.hpp"
#include "input.hpp"

namespace fr {

class Dodge {
	public:
	Dodge();
	Dodge(Control control, Action action, int self_damage, float active_begin,
			float active_end, float recovery_end, sf::Vector2f offset,
			Animations animation);

	void start();
	void interrupt();
	void end();
	bool isStartingUp() const;
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;

	float progress = -1;
	Control control;
	Action action;
	int self_damage;
	sf::Vector2f offset;
	Animations animation;

	private:
	float active_begin;
	float active_end;
	float recovery_end;
};

} // namespace fr
