#include "punch.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

// Empty constructor
fr::Punch::Punch() {
}

fr::Punch::Punch(Control control, Control mod, float min_held_time,
		bool lead_handed, bool body_shot, int damage, int perm_hit_damage,
		int block_damage, int self_damage, float interrupt_end,
		float hitbox_begin, float hitbox_end, float recovery_end,
		sf::IntRect hitbox, sf::IntRect clearbox)
		: control(control), mod(mod), min_held_time(min_held_time),
		lead_handed(lead_handed), body_shot(body_shot), hit_damage(hit_damage),
		perm_hit_damage(perm_hit_damage), block_damage(block_damage),
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

bool fr::Punch::interruptible() const {
	return progress <= interrupt_end && !done();
}

bool fr::Punch::active() const {
	return progress > hitbox_begin && !recovering() && !done();
}

bool fr::Punch::recovering() const {
	return progress > hitbox_end && !done();
}

bool fr::Punch::done() const {
	return progress > recovery_end || progress == -1;
}

sf::IntRect fr::Punch::getHitbox(sf::Vector2f relative_to, int direction) const {
	int left = relative_to.x + hitbox.left;
	int top = relative_to.y + hitbox.top;
	return sf::IntRect(left, top, hitbox.width, hitbox.height);
}

sf::IntRect fr::Punch::getClearbox(sf::Vector2f relative_to, int direction) const {
	int left = relative_to.x + clearbox.left;
	int top = relative_to.y + clearbox.top;
	return sf::IntRect(left, top, clearbox.width, clearbox.height);
}

std::vector<fr::Punch> fr::default_punches = {
};
