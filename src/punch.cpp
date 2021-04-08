#include "punch.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

fr::Punch::Punch() {} // Empty constructor

fr::Punch::Punch(Control control, Action action, Control mod,
		float min_held, float max_held, int damage, int perma_damage,
		int block_damage, int self_damage, float interrupt_end,
		float hitbox_begin, float hitbox_end, float recovery_end,
		bool needs_clear, sf::FloatRect hitbox)
		: control(control), action(action), mod(mod),
		min_held(min_held), max_held(max_held), damage(damage),
		perma_damage(perma_damage), block_damage(block_damage),
		self_damage(self_damage), interrupt_end(interrupt_end),
		hitbox_begin(hitbox_begin), hitbox_end(hitbox_end),
		recovery_end(recovery_end), needs_clear(needs_clear), hitbox(hitbox) {
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
	return progress >= hitbox_end && !isDone();
}

bool fr::Punch::isDone() const {
	return progress > recovery_end || progress == -1;
}

sf::FloatRect fr::Punch::getHitbox(sf::FloatRect relative_to, int direction) const {
	int left;
	if (direction == 1)
		left = relative_to.left + hitbox.left;
	else
		left = relative_to.left - hitbox.left + relative_to.width - hitbox.width;
	int top = relative_to.top + hitbox.top;
	return sf::FloatRect(left, top, hitbox.width, hitbox.height);
}

std::vector<fr::Punch> fr::default_punches = {
	// Body jab
	Punch(Control::a, Action::release, Control::down, 0, PRESS_END,
			1, 0, 0, 1, 0.15f, 0.2f, 0.25f, 0.3f, true,
			sf::FloatRect(32, 20, 24, 8)),
	// Head jab
	Punch(Control::a, Action::release, Control::none, 0, PRESS_END,
			2, 0, 0, 1, 0.15f, 0.2f, 0.25f, 0.3f, true,
			sf::FloatRect(32, 8, 24, 8)),

	// Body cross
	Punch(Control::a, Action::hold, Control::down, PRESS_END, PRESS_END + 0.1f,
			2, 1, 1, 1, 0.15f, 0.2f, 0.25f, 0.5f, true,
			sf::FloatRect(32, 20, 26, 8)),

	// Head cross
	Punch(Control::a, Action::hold, Control::none, PRESS_END, PRESS_END + 0.1f,
			4, 0, 1, 1, 0.15f, 0.3f, 0.35f, 0.5f, true,
			sf::FloatRect(32, 8, 26, 8)),

	// Body upper
	Punch(Control::b, Action::release, Control::down, 0, PRESS_END,
			3, 2, 1, 1, 0.15f, 0.325f, 0.375f, 0.625f, false,
			sf::FloatRect(48, 20, 24, 12)),

	// Head upper
	Punch(Control::b, Action::release, Control::none, 0, PRESS_END,
			4, 0, 1, 1, 0.15f, 0.325f, 0.375f, 0.625f, true,
			sf::FloatRect(48, 4, 24, 12)),

	// Body hook
	Punch(Control::b, Action::hold, Control::down, PRESS_END, PRESS_END + 0.1f,
			2, 2, 2, 2, 0.1f, 0.35125f, 0.4125f, 0.75f, false,
			sf::FloatRect(64, 20, 16, 12)),

	// Head hook
	Punch(Control::b, Action::hold, Control::none, PRESS_END, PRESS_END + 0.1f,
			6, 0, 2, 2, 0.1f, 0.35125f, 0.4125f, 0.75f, false,
			sf::FloatRect(64, 4, 16, 12)),
};
