#include "punch.hpp"

#include <vector>

#include <toml++/toml.h>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

bool fr::Punch::isStartingUp(float progress) const {
	return progress < hitbox_begin;
}

bool fr::Punch::isUnstoppable(float progress) const {
	return progress > hitbox_begin * UNSTOPPABLE_AFTER;
}

bool fr::Punch::isActive(float progress) const {
	return progress > hitbox_begin && !isRecovering(progress);
}

bool fr::Punch::isRecovering(float progress) const {
	return progress > hitbox_end && !isDone(progress);
}

bool fr::Punch::isDone(float progress) const {
	return progress > recovery_end;
}

sf::IntRect fr::Punch::getHitbox(sf::Vector2f relative_to) const {
	int left = relative_to.x + hitbox.left;
	int top = relative_to.y + hitbox.top;
	return sf::IntRect(top, left, hitbox.width, hitbox.height);
}
