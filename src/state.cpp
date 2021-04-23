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
		
	if (getPunch().isDone())
		punch = Punches::none;
		
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
		if (input.control == Controls::left)
			movement = Movements::walk_l;

		if (input.control == Controls::right)
			movement = Movements::walk_r;

		// Guard
		if (input.control == Controls::up && input.action == Actions::hold)
			guard = Guards::head;
		else if (input.control == Controls::down && input.action == Actions::hold)
			guard = Guards::body;
		else
			guard = Guards::none;

		// Dodge
		if (input.control == Controls::left && input.action == Actions::double_press)
			dodge = Dodges::pull;

		// Idle
		if (input.action == Actions::release)
			movement = Movements::idle;
	}
}

fr::Punch &fr::State::getPunch() {
	return punches[(int)punch];
}

bool fr::State::isPunching() {
	return punch != Punches::none;
}
