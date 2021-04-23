#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"
#include "animations.hpp"

namespace fr {

class Dodge {
	public:
	Dodge();
	Dodge(Controls control, Actions action, sf::Vector2f offset,
			float active_begin, float active_end, float recovery_end,
			Animations animation);

	void start();
	void interrupt();
	void end();
	bool isStartingUp() const;
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;

	float progress = -1;
	Controls control;
	Actions action;
	sf::Vector2f offset;
	Animations animation;

	private:
	float active_begin;
	float active_end;
	float recovery_end;
};

extern std::vector<Dodge> default_dodges;

} // namespace fr
