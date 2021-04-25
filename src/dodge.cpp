#include "dodge.hpp"

#include <vector>

#include "SFML/System.hpp"
#include "input.hpp"
#include "animations.hpp"

fr::Dodge::Dodge() {} // Empty constructor

fr::Dodge::Dodge(Controls control, Actions action, sf::Vector2f offset,
		float active_begin, float active_end, float recovery_end,
		fr::Animations animation)
		: control(control), action(action), offset(offset), 	
		active_begin(active_begin), active_end(active_end),
		recovery_end(recovery_end), animation(animation) {
}

void fr::Dodge::start() {
	progress = 0;
}

void fr::Dodge::interrupt() {
	progress = active_end;
}

void fr::Dodge::end() {
	progress = -1;
}

bool fr::Dodge::isStartingUp() const {
	return !isActive() && !isRecovering() && !isDone();
}

bool fr::Dodge::isActive() const {
	return progress > active_begin && !isRecovering() && !isDone();
}

bool fr::Dodge::isRecovering() const {
	return progress >= active_end && !isDone();
}

bool fr::Dodge::isDone() const {
	return progress > recovery_end || progress == -1;
}

std::vector<fr::Dodge> fr::default_dodges = {
	// None
	Dodge(),
	
	// Pull
	Dodge(Controls::left, Actions::double_press, sf::Vector2f(-8, 0), 0, 
			0.25f, 0.3f, Animations::pull),
			
	Dodge(Controls::up, Actions::double_press, sf::Vector2f(999, 0), 0, 
			0.225f, 0.225f, Animations::slip),
			
	Dodge(Controls::down, Actions::double_press, sf::Vector2f(2, 8), 0.025f, 
			0.275f, 0.3f, Animations::duck),
};
