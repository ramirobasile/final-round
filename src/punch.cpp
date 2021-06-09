#include "punch.hpp"

#include <vector>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "animation.hpp"
#include "direction.hpp"
#include "input.hpp"

// Head punch constructor
fr::Punch::Punch(fr::Control control, fr::Hit hit, int cost, float hit_range, 
		float stuff_range, float interrupt_end, float active_begin, 
		float active_end, float recovery_end, fr::Animation animation, 
		sf::Sound sound)
		: control(control), mod(Control::none), hit(hit), 
		cost(cost), hit_range(hit_range), stuff_range(stuff_range), 
		interrupt_end(interrupt_end), active_begin(active_begin), 
		active_end(active_end), recovery_end(recovery_end), 
		animation(animation), sound(sound) {
}
		
// Body punch constructor
fr::Punch::Punch(fr::Punch head_punch, fr::Hit hit, float hit_range, 
		float stuff_range, fr::Animation animation)
		: control(head_punch.control), mod(BODY_MOD), hit(hit), 
		cost(head_punch.cost), hit_range(hit_range), stuff_range(stuff_range), 
		interrupt_end(head_punch.interrupt_end), 
		active_begin(head_punch.active_begin), active_end(head_punch.active_end), 
		recovery_end(head_punch.recovery_end), animation(animation), 
		sound(head_punch.sound) {
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
		progress = active_end;
}

bool fr::Punch::canInterrupt() const {
	return progress <= interrupt_end && !isDone();
}

bool fr::Punch::isActive() const {
	return progress > active_begin && !isRecovering() && !isDone();
}

bool fr::Punch::isRecovering() const {
	return progress >= active_end && !isDone();
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

int fr::Punch::getCost() const {
	return cost;
}

bool fr::Punch::hits(sf::FloatRect hurtbox, sf::FloatRect relative_to, 
		fr::Direction direction) const {
	float left = relative_to.left;
	
	if (direction == Direction::left)
		left = left - relative_to.width - hit_range;

	sf::FloatRect hitbox(left, relative_to.top, hit_range, 1);

	return isActive() && hitbox.intersects(hurtbox);
}

bool fr::Punch::getsStuffed(sf::FloatRect hurtbox, sf::FloatRect relative_to, 
		fr::Direction direction) const {
	float left = relative_to.left;
	
	if (direction == Direction::left)
		left = left - relative_to.width - stuff_range;

	sf::FloatRect stuffbox(left, relative_to.top, stuff_range, 1);

	return isActive() && stuffbox.intersects(hurtbox);
}
