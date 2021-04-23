#include "state.hpp"

#include <iostream>
#include <vector>

#include "input.hpp"
#include "punch.hpp"
#include "dodge.hpp"
#include "physics.hpp"
#include "animations.hpp"

fr::State::State() {} // Empty constructor

fr::State::State(std::vector<fr::Punch> punches, std::vector<fr::Dodge> dodges)
		: punches(punches), dodges(dodges) {
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
		for (int j = 0; j < punches.size() && punch.isDone(); ++j) {
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
		for (int j = 0; j < dodges.size() && dodge.isDone(); ++j) {
			bool same_control = input.control == dodges[j].control;
			bool same_action = input.action == dodges[j].action;

			if (same_control && same_action) {
				dodge.end();
				dodge = dodges[j];
				dodge.start();
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

		// Idle
		if (input.action == Actions::release)
			movement = Movements::idle;
	}
}
