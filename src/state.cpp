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

void fr::State::update(std::vector<fr::Input> inputs,
		std::vector<fr::Input> buffer, std::vector<fr::Punch> punches) {
	punch_progress += dt;

	// TODO Something like this
	//if (!isPunching(punch_progress))
	//	movement = Movement::idle;

	for (int i = 0; i < inputs.size(); ++i) {
		switch (inputs[i].action) {
			case Action::press:
				onPress(inputs[i], buffer, punches);
				break;
				
			case Action::release:
				onRelease(inputs[i], buffer, punches);
				break;
				
			case Action::hold:
				onHold(inputs[i], buffer, punches);
				break;
		}
	}
}

bool fr::State::isPunching() const {
	return !punch.isDone(punch_progress);
}

bool fr::State::isLeadFree() const {
	// TODO Have seperate lead and rear punches
	//return (punch.isRecovering(rear_punch_progress) && !punch.is_lead_handed)
	//		|| !isPunching();
	return !isPunching();
}

bool fr::State::isRearFree() const {
	// TODO Have seperate lead and rear punches
	//return (punch.isRecovering(lead_punch_progress) && punch.is_lead_handed)
	//		|| !isPunching();
	return !isPunching();
}

bool fr::State::isHeadGuardUp(std::vector<fr::Input> buffer) const {
	return !isPunching() && !isBuffered(BODY_CONTROL, buffer);
}

bool fr::State::isBodyGuardUp(std::vector<fr::Input> buffer) const {
	return !isPunching() && isBuffered(BODY_CONTROL, buffer);
}

void fr::State::onHold(fr::Input input, std::vector<fr::Input> buffer,
		std::vector<fr::Punch> punches) {
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
				bool hand = (candidate.is_lead_handed && isLeadFree())
						|| (!candidate.is_lead_handed && isRearFree());
				bool body = candidate.is_body 
						== isBuffered(BODY_CONTROL, buffer);
						
				if (control && hand && body) {
					punch = candidate;
					movement = Movement::punch;
					punch_progress = 0;
				}
			}
			break;
		
		case Control::dodge:
			if (isPunching() && !punch.isUnstoppable(punch_progress)) {
				punch_progress = punch.recovery_end;
			} else {
			}
	}
}

void fr::State::onRelease(fr::Input input, std::vector<fr::Input> buffer,
		std::vector<fr::Punch> punches) {
	switch (input.control) {
		default:
			movement = Movement::idle;
			break;
	}
}
