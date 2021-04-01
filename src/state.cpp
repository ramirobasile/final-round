#include "state.hpp"

#include <iostream>
#include <vector>

#include "main.hpp"
#include "input.hpp"
#include "punch.hpp"
#include "physics.hpp"

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

	for (int i = 0; i < inputs.size(); ++i) {
		for (int j = 0; j < punches.size(); ++j) {
			bool same_control = inputs[i].control == punches[j].control;
			bool same_action = inputs[i].action == punches[j].action;
			bool mod_buffered = buffered(punches[j].mod, buffer);

			if (!punching() && same_control && same_action && mod_buffered) {
				punch.end();
				punch = punches[j];
				punch.start();
			}
		}
	}
}

bool fr::State::punching() {
	return !punch.done();
}

bool fr::State::guarding() {
	return guard_high || guard_low;
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
	}
}

void fr::State::onRelease(fr::Input input, std::vector<fr::Input> buffer) {
	switch (input.control) {
		default:
			movement = Movement::idle;
			break;
	}
}
