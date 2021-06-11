#include "health.hpp"

#include <algorithm>

fr::Health::Health(int max, int min, int health_regen, float regen_rate) 
		: max(max), min(min), health_regen(health_regen), 
		regen_rate(regen_rate), current(max) {
}

void fr::Health::regen(float dt) {
	tt_regen += dt;
	
	if (tt_regen > regen_rate) {
		takeDamage(-health_regen);
		tt_regen = 0;
	}
}

void fr::Health::takeDamage(int damage) {
	current = std::clamp(current - damage, min, max);
}

void fr::Health::takePermaDamage(int damage) {
	max = std::clamp(max - damage, min, max);
	current = std::clamp(current, min, max);
}

void fr::Health::resetRegen() {
	tt_regen = 0;
}

int fr::Health::getMin() const {
	return min;
}

int fr::Health::getMax() const {
	return max;
}

int fr::Health::getCurrent() const {
	return current;
}
