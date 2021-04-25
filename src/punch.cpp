#include "punch.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "animations.hpp"

fr::Punch::Punch() {} // Empty constructor

fr::Punch::Punch(Controls control, Actions action, Controls mod,
		float min_held, float max_held, int damage, int perma_damage,
		int block_damage, int self_damage, float interrupt_end,
		float hitbox_begin, float hitbox_end, float recovery_end,
		bool needs_clear, sf::FloatRect hitbox, fr::Animations animation)
		: control(control), action(action), mod(mod),
		min_held(min_held), max_held(max_held), damage(damage),
		perma_damage(perma_damage), block_damage(block_damage),
		self_damage(self_damage), interrupt_end(interrupt_end),
		hitbox_begin(hitbox_begin), hitbox_end(hitbox_end),
		recovery_end(recovery_end), needs_clear(needs_clear), hitbox(hitbox),
		animation(animation) {
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
	// None
	Punch(),
	
	// Body jab
	Punch(Controls::a, Actions::release, Controls::down, 0, PRESS_END,
			1, 0, 0, 1, 0.15f, 0.2f, 0.25f, 0.3f, true,
			sf::FloatRect(32, 20, 24, 8), Animations::jab_body),
	// Head jab
	Punch(Controls::a, Actions::release, Controls::none, 0, PRESS_END,
			2, 0, 0, 1, 0.15f, 0.2f, 0.25f, 0.3f, true,
			sf::FloatRect(32, 8, 24, 8), Animations::jab_head),

	// Body cross
	Punch(Controls::a, Actions::hold, Controls::down, PRESS_END, PRESS_END + 0.1f,
			2, 1, 1, 1, 0.15f, 0.2f, 0.25f, 0.5f, true,
			sf::FloatRect(32, 20, 24, 8), Animations::cross_body),

	// Head cross
	Punch(Controls::a, Actions::hold, Controls::none, PRESS_END, PRESS_END + 0.1f,
			4, 0, 1, 1, 0.15f, 0.275f, 0.375f, 0.5f, true,
			sf::FloatRect(32, 8, 24, 8), Animations::cross_head),

	// Body upper
	Punch(Controls::b, Actions::release, Controls::down, 0, PRESS_END,
			3, 2, 1, 1, 0.15f, 0.3125f, 0.35f, 0.625f, false,
			sf::FloatRect(24, 20, 24, 12), Animations::upper_body),

	// Head upper
	Punch(Controls::b, Actions::release, Controls::none, 0, PRESS_END,
			4, 0, 1, 1, 0.15f, 0.3125f, 0.35f, 0.625f, true,
			sf::FloatRect(24, 4, 24, 12), Animations::upper_head),

	// Body hook
	Punch(Controls::b, Actions::hold, Controls::down, PRESS_END, PRESS_END + 0.1f,
			2, 2, 2, 2, 0.1f, 0.325f, 0.425f, 0.75f, false,
			sf::FloatRect(26, 20, 16, 12), Animations::hook_body),

	// Head hook
	Punch(Controls::b, Actions::hold, Controls::none, PRESS_END, PRESS_END + 0.1f,
			6, 0, 2, 2, 0.1f, 0.325f, 0.425f, 0.75f, false,
			sf::FloatRect(26, 4, 16, 12), Animations::hook_head),
};
