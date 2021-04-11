#include "state.hpp"

#include <iostream>
#include <vector>

#include "input.hpp"
#include "punch.hpp"
#include "physics.hpp"

fr::State::State() {} // Empty constructor

fr::State::State(std::vector<Punch> punches) : punches(punches) {
}

void fr::State::update(std::vector<fr::Input> inputs, 
		std::vector<fr::Input> buffer, float dt) {
	if (isPunching())
		getPunch().progress += dt;
		
	for (int i = 0; i < inputs.size(); ++i) {
		Input input = inputs[i];

		// Punch
		for (int j = 0; j < punches.size() && !isPunching(); ++j) {
			bool same_control = input.control == punches[j].control;
			bool same_action = input.action == punches[j].action;
			// TODO Change to has input rather than input buffered
			bool mod_inputted = inputted(punches[j].mod, inputs);
			bool held_margin = input.held >= punches[j].min_held
					&& (input.held < punches[j].max_held
					|| punches[j].max_held == -1);

			if (same_control && same_action && mod_inputted && held_margin) {
				getPunch().end();
				punch = (Punches)j;
				getPunch().start();
			}
		}

		// Walk
		if (input.control == Control::left)
			movement = Movements::walk_l;

		if (input.control == Control::right)
			movement = Movements::walk_r;

		// Guard
		guard_high = false;
		guard_low = false;
		if (input.control == Control::up && input.action == Action::hold)
			guard_high = true;
		else if (input.control == Control::down && input.action == Action::hold)
			guard_low = true;

		// Idle
		if (input.action == Action::release)
			movement = Movements::idle;
	}
}

fr::Punch &fr::State::getPunch() {
	return punches[(int)punch];
}

bool fr::State::isPunching() {
	return !getPunch().isDone();
}

bool fr::State::isGuarding() const {
	return guard_high || guard_low;
}
