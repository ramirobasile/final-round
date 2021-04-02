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
		Input input = inputs[i];

		// Punch
		for (int j = 0; j < punches.size() && !punching(); ++j) {
			bool same_control = input.control == punches[j].control;
			bool same_action = input.action == punches[j].action;
			// TODO Change to has input rather than input buffered
			bool mod_inputted = inputted(punches[j].mod, inputs);
			bool held_margin = input.held >= punches[j].min_held
					&& (input.held < punches[j].max_held
					|| punches[j].max_held == -1);

			if (same_control && same_action && mod_inputted && held_margin) {
				punch.end();
				punch = punches[j];
				punch.start();
			}
		}

		// Walk
		if (input.control == Control::left)
			movement = Movement::walk_l;

		if (input.control == Control::right)
			movement = Movement::walk_r;

		// Idle
		if (input.action == Action::release)
			movement = Movement::idle;
	}
}

bool fr::State::punching() {
	return !punch.done();
}

bool fr::State::guarding() {
	return guard_high || guard_low;
}
