#include "state.hpp"

#include <iostream>
#include <vector>

#include "main.hpp"
#include "input.hpp"
#include "punch.hpp"
#include "physics.hpp"

// HACK Why not fr::State? I don't think I understand this fully
std::ostream& fr::operator<<(std::ostream &out, const fr::State &state) {
	out << "State: "
		<< "Movement " << std::to_string((int)state.movement);
    return out;
}

bool fr::State::operator==(const fr::State &state) const {
	return this->movement == state.movement;
}

fr::Punch fr::State::getPunch() {
	return default_punches[punch];
}

bool fr::State::isPunching() {
	return movement == Movement::punch;
}
bool fr::State::isLeadFree() {
	return false;
}

bool fr::State::isRearFree() {
	return false;
}

bool fr::State::isBodyOpen() {
	return false;
}

void fr::State::update(std::vector<fr::Input> inputs,
		std::vector<fr::Input> buffer) {
	punch_progress += dt;
	
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

void fr::State::onPress(fr::Input input, std::vector<fr::Input> buffer) {
	switch (input.control) {
		case Control::jab:
		case Control::cross:
		case Control::upper:
		case Control::hook:
			for (int i = 0; i < default_punches.size() && !isPunching(); ++i) {
				Punch candidate = default_punches[i];
				bool control = input.control == candidate.control;
				bool hand = (candidate.is_lead_handed && isLeadFree())
						|| (!candidate.is_lead_handed && isRearFree());
				bool body = candidate.is_body 
						== isBuffered(candidate.BODY_CONTROL, buffer);
						
				if (control && hand && body) {
					punch = i;
					movement = Movement::punch;
					punch_progress = 0;
				}
			}
			break;
		
		case Control::dodge:
			if (isPunching() && !getPunch().isUnstoppable(punch_progress)) {
				punch_progress = getPunch().recovery_end;
			} else {
			}
	}
}

void fr::State::onRelease(fr::Input input, std::vector<fr::Input> buffer) {
	switch (input.control) {
		default:
			movement = Movement::idle;
			break;
	}
}
