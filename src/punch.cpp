#include "punch.hpp"

#include <vector>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "animation.hpp"
#include "direction.hpp"
#include "input.hpp"

fr::Punch::Punch(fr::Control control, fr::Control mod, fr::Hit hit,
		int cost, float interrupt_end, float hitbox_begin, float hitbox_end,
		float recovery_end, sf::FloatRect hitbox, sf::FloatRect clearbox,
		fr::Animation animation, sf::Sound sound)
		: control(control), mod(mod), hit(hit), cost(cost),
		interrupt_end(interrupt_end), hitbox_begin(hitbox_begin),
		hitbox_end(hitbox_end), recovery_end(recovery_end), hitbox(hitbox),
		clearbox(clearbox), animation(animation), sound(sound) {
}

void fr::Punch::update(float dt) {
	if (!isDone())
		progress += dt;
}

void fr::Punch::reset() {
	progress = 0;
}

void fr::Punch::end() {
	progress = -1;
}

void fr::Punch::interrupt() {
	if (!isDone())
		progress = hitbox_end;
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
	return progress == -1 || progress > recovery_end;
}

fr::Control fr::Punch::getControl() const {
	return control;
}

fr::Control fr::Punch::getMod() const {
	return mod;
}

fr::Hit fr::Punch::getHit() const {
	return hit;
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

sf::FloatRect fr::Punch::getClearbox(sf::FloatRect relative_to,
		fr::Direction direction) const {
	int left;
	if (direction == Direction::right)
		left = relative_to.left + clearbox.left;
	else
		left = relative_to.left - clearbox.left + relative_to.width - clearbox.width;

	int top = relative_to.top + clearbox.top;

	return sf::FloatRect(left, top, clearbox.width, clearbox.height);
}

int fr::Punch::getCost() const {
	return cost;
}
