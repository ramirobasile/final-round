#include "punch.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

// Empty constructor
fr::Punch::Punch() {
}

fr::Punch::Punch(Control control, Action action, Control mod,
		float min_held, float max_held, int damage, int perma_damage,
		int block_damage, int self_damage, float interrupt_end,
		float hitbox_begin, float hitbox_end, float recovery_end,
		sf::IntRect hitbox, sf::IntRect clearbox)
		: control(control), action(action), mod(mod),
		min_held(min_held), max_held(max_held), damage(damage),
		perma_damage(perma_damage), block_damage(block_damage),
		self_damage(self_damage), interrupt_end(interrupt_end),
		hitbox_begin(hitbox_begin), hitbox_end(hitbox_end),
		recovery_end(recovery_end), hitbox(hitbox), clearbox(clearbox) {
}

void fr::Punch::start() {
	progress = 0;
}

void fr::Punch::interrupt() {
	progress = hitbox_end;
}

void fr::Punch::end() {
	progress = -1;
}

bool fr::Punch::canInterrupt() const {
	return progress <= interrupt_end && !isDone();
}

bool fr::Punch::isActive() const {
	return progress > hitbox_begin && !isRecovering() && !isDone();
}

bool fr::Punch::isRecovering() const {
	return progress > hitbox_end && !isDone();
}

bool fr::Punch::isDone() const {
	return progress > recovery_end || progress == -1;
}

sf::IntRect fr::Punch::getHitbox(sf::IntRect relative_to, int direction) const {
	int left;
	if (direction == 1)
		left = relative_to.left + hitbox.left;
	else
		left = relative_to.left - hitbox.left + relative_to.width - hitbox.width;
	int top = relative_to.top + hitbox.top;
	return sf::IntRect(left, top, hitbox.width, hitbox.height);
}

sf::IntRect fr::Punch::getClearbox(sf::IntRect relative_to, int direction) const {
	int left;
	if (direction == 1)
		left = relative_to.left + clearbox.left;
	else
		left = relative_to.left - clearbox.left + relative_to.width - clearbox.width;
	int top = relative_to.top + clearbox.top;
	return sf::IntRect(left, top, clearbox.width, clearbox.height);
}

std::vector<fr::Punch> fr::default_punches = {
	// Body jab
	Punch(Control::a, Action::release, Control::down, 0, PRESS_END,
			1, 0, 0, 1, 0.15f, 0.25f, 0.3f, 0.35f,
			sf::IntRect(96, 40, 72, 24), sf::IntRect(96, 40, 20, 24)),
	// Head jab
	Punch(Control::a, Action::release, Control::none, 0, PRESS_END,
			2, 0, 0, 1, 0.15f, 0.25f, 0.3f, 0.35f,
			sf::IntRect(96, 8, 72, 24), sf::IntRect(96, 8, 20, 24)),

	// Body cross
	Punch(Control::a, Action::hold, Control::down, PRESS_END, PRESS_END + 0.1f,
			2, 1, 1, 1, 0.175f, 0.3f, 0.35f, 0.4f,
			sf::IntRect(96, 40, 72, 24), sf::IntRect(96, 40, 20, 24)),

	// Head cross
	Punch(Control::a, Action::hold, Control::none, PRESS_END, PRESS_END + 0.1f,
			4, 0, 1, 1, 0.175f, 0.3f, 0.35f, 0.4f,
			sf::IntRect(96, 8, 72, 24), sf::IntRect(96, 8, 20, 24)),

	// Body upper
	Punch(Control::b, Action::release, Control::down, 0, PRESS_END,
			3, 2, 1, 1, 0.15f, 0.325f, 0.375f, 0.525f,
			sf::IntRect(96, 40, 48, 24), sf::IntRect(0, 0, 0, 0)),

	// Head upper
	Punch(Control::b, Action::release, Control::none, 0, PRESS_END,
			4, 0, 1, 1, 0.15f, 0.325f, 0.375f, 0.525f,
			sf::IntRect(96, 8, 48, 24), sf::IntRect(0, 0, 0, 0)),

	// Body hook
	Punch(Control::b, Action::hold, Control::down, PRESS_END, PRESS_END + 0.1f,
			2, 2, 2, 2, 0.1f, 0.35125f, 0.4125f, 0.5125f,
			sf::IntRect(128, 40, 32, 24), sf::IntRect(0, 0, 0, 0)),

	// Head hook
	Punch(Control::b, Action::hold, Control::none, PRESS_END, PRESS_END + 0.1f,
			6, 0, 2, 2, 0.1f, 0.35125f, 0.4125f, 0.5125f,
			sf::IntRect(128, 8, 32, 24), sf::IntRect(0, 0, 0, 0)),
};
