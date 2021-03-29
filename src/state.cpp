#include "state.hpp"

#include <iostream>
#include <vector>

#include "main.hpp"
#include "input.hpp"
#include "punch.hpp"
#include "physics.hpp"

// TODO Complete
std::ostream& fr::operator<<(std::ostream &out, const fr::State &state) {
	out << "State: "
		<< "Movement " << std::to_string((int)state.movement);
    return out;
}

// TODO Punch operator==
bool fr::State::operator==(const fr::State &state) const {
	return this->movement == state.movement;
}

void fr::State::update(std::vector<fr::Input> inputs,
		std::vector<fr::Input> buffer, std::vector<fr::Punch> punches) {
	if (punching())
		punch.progress += dt;

	for (int i = 0; i < inputs.size(); ++i) {
		switch (inputs[i].action) {
			case Action::press:
				onPress(inputs[i], buffer, punches);
				break;
				
			case Action::release:
				onRelease(inputs[i], buffer);
				break;
				
			case Action::hold:
				onHold(inputs[i], buffer);
				break;
		}
	}
}

bool fr::State::punching() {
	return !punch.done();
}

void fr::State::onHold(fr::Input input, std::vector<fr::Input> buffer) {
	switch (input.control) {
		case Control::left:
			movement = Movement::walk_l;
			break;

		case Control::right:
			movement = Movement::walk_r;
			break;
	}
}

void fr::State::onPress(fr::Input input, std::vector<fr::Input> buffer,
			std::vector<fr::Punch> punches) {
	switch (input.control) {
		case Control::lead:
		case Control::rear:
			for (int i = 0; i < punches.size(); ++i) {
				Punch candidate = punches[i];
				bool same_control = input.control == candidate.control;
				bool held_enough = input.held_time >= candidate.min_held_time;
				bool same_hand = punching() && punch.lead_handed == candidate.lead_handed;
				bool can_throw = !punching() || (punch.interruptible() && !same_hand);
				bool mod_buffered = buffered(candidate.mod, buffer);
						
				if (same_control && held_enough && can_throw && mod_buffered) {
					punch.end();
					punch = candidate;
					punch.start();
				}
			}
			break;
		
		case Control::dodge:
			// TODO Feint
			break;
	}
}

void fr::State::onRelease(fr::Input input, std::vector<fr::Input> buffer) {
	switch (input.control) {
		default:
			movement = Movement::idle;
			break;
	}
}
