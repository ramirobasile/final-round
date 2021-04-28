#include "punch.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "input.hpp"
#include "animations.hpp"

fr::Punch::Punch() {} // Empty constructor

fr::Punch::Punch(Control control, Action action, Control mod,
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

sf::FloatRect fr::Punch::getHitbox(sf::FloatRect relative_to, 
		fr::Direction direction) const {
	int left;
	if (direction == Direction::right)
		left = relative_to.left + hitbox.left;
	else
		left = relative_to.left - hitbox.left + relative_to.width - hitbox.width;
	int top = relative_to.top + hitbox.top;
	return sf::FloatRect(left, top, hitbox.width, hitbox.height);
}
