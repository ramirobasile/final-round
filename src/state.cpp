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
	punch.progress += dt;

	for (int i = 0; i < inputs.size(); ++i) {
		switch (inputs[i].action) {
			case Action::press:
				onPress(inputs[i], buffer);
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
		case Control::backwards:
			movement = Movement::walk_b;
			break;

		case Control::forwards:
			movement = Movement::walk_f;
			break;
	}
}

void fr::State::onPress(fr::Input input, std::vector<fr::Input> buffer,
			std::vector<fr::Punch> punches) {
	switch (input.control) {
		case Control::jab:
		case Control::cross:
		case Control::upper:
		case Control::hook:
			for (int i = 0; i < punches.size(); ++i) {
				Punch candidate = punches[i];
				bool control = input.control == candidate.control;

				bool same_hand = punching()
						&& punch.lead_handed == candidate.lead_handed;
				bool can_throw = punching()
						|| (punch.interruptible() && !same_hand);

				bool body = candidate.body_shot == buffered(BODY_CONTROL, buffer);
						
				if (control && can_throw && body) {
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
