#include "dodge.hpp"

#include "animation.hpp"

fr::Dodge::Dodge(int cost, float active_begin, float active_end,
		float recovery_end, fr::Animation animation)
		: cost(cost), active_begin(active_begin), active_end(active_end),
		recovery_end(recovery_end), animation(animation) {
}

void fr::Dodge::update(float dt) {
	if (!isDone())
		progress += dt;
}

void fr::Dodge::reset() {
	progress = 0;
}

void fr::Dodge::interrupt() {
	progress = active_end;
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

float fr::Dodge::getCost() const {
	return cost;
}
