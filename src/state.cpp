#include "state.hpp"

#include <iostream>
#include <vector>

#include "animations.hpp"
#include "dodge.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "punch.hpp"

fr::State::State() {} // Empty constructor

fr::State::State(std::vector<fr::Punch> punches, fr::Dodge dodge)
		: punches(punches), dodge(dodge) {
}

void fr::State::update(std::vector<fr::Input> inputs, 
		std::vector<fr::Input> buffer, float dt) {
	if (!punch.isDone())
		punch.progress += dt;
		
	if (!dodge.isDone())
		dodge.progress += dt;
		
	for (int i = 0; i < inputs.size(); ++i) {
		Input input = inputs[i];

		// Punch
		for (int j = 0; j < punches.size() && punch.isDone() && dodge.isDone(); ++j) {
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
		
		// Dodge
		if (dodge.isDone() && input.control == Control::backwards 
				&& input.action == Action::double_press) {
			dodge.start();
		}

		// Walk
		if (input.control == Control::left)
			movement = Movements::walk_l;

		if (input.control == Control::right)
			movement = Movements::walk_r;

		// Guard
		if (input.control == Control::up && input.action == Action::hold)
			guard = Guards::head;
		else if (input.control == Control::down && input.action == Action::hold)
			guard = Guards::body;
		else
			guard = Guards::none;

		// Idle
		if (input.action == Action::release)
			movement = Movements::idle;
	}
}
