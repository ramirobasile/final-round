#include "dodge.hpp"

fr::Dodge::Dodge() {} // Empty constructor

fr::Dodge::Dodge(int self_damage, float active_begin, float active_end, 
		float recovery_end)
		: self_damage(self_damage), active_begin(active_begin), 
		active_end(active_end), recovery_end(recovery_end) {
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
