#include "punch.hpp"

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"

fr::Punch::Punch(fr::Control control, sf::IntRect hitbox, bool is_lead_handed,
		bool is_body, int damage, int body_hp_cost, float hitbox_begin,
		float hitbox_end, float recovery_end)
		: control(control), hitbox(hitbox), is_lead_handed(is_lead_handed),
		is_body(is_body), damage(damage), body_hp_cost(body_hp_cost), 
		hitbox_begin(hitbox_begin), hitbox_end(hitbox_end),
		recovery_end(recovery_end) {
}

sf::IntRect fr::Punch::getHitbox(sf::Vector2f relative_to) {
	int left = hitbox.left + relative_to.left;
	int top = hitbox.top + relative_to.top;
	return sf::IntRect(top, left, hitbox.width, hitbox.height);
}

bool fr::Punch::isActive(float progress) {
	return progress > hitbox_begin;
}

bool fr::Punch::isUnstoppable(float progress) {
	return progress > hitbox_begin * UNSTOPPABLE_AFTER;
}

std::vector<fr::Punch> fr::default_punches = {
	// TODO Body shots
	
	// Lead jab
	Punch(Control::jab, sf::IntRect(000, 000, 000, 000), 
			true, false, 4, 1, 0.1f, 0.15f, 0.2f),
		
	// Lead cross
	Punch(Control::cross, sf::IntRect(000, 000, 000, 000), 
			false, false, 6, 2, 0.15f, 0.2f, 0.3f),
			
	// Rear cross
	Punch(Control::cross, sf::IntRect(000, 000, 000, 000), 
			false, false, 12, 2, 0.15f, 0.2f, 0.3f),
			
	// Lead hook
	Punch(Control::hook, sf::IntRect(000, 000, 000, 000), 
			true, false, 20, 3, 0.2f, 0.25f, 0.35f),
			
	// Rear hook
	Punch(Control::hook, sf::IntRect(000, 000, 000, 000), 
			false, false, 20, 3, 0.2125f, 0.2625f, 0.3625f),
			
	// Rear upper
	Punch(Control::upper, sf::IntRect(000, 000, 000, 000), 
			false, false, 20, 3, 0.2f, 0.25f, 0.35f),
			
	// Lead upper
	Punch(Control::upper, sf::IntRect(000, 000, 000, 000), 
			true, false, 20, 3, 0.2f, 0.25f, 0.35f),
};
