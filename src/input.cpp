#include "input.hpp"

#include <iostream>
#include <vector>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"

// HACK Why not fr::Input::? I don't think I understand this fully
std::ostream& fr::operator<<(std::ostream &out, const fr::Input &input) {
	out	<< "Input: "
		<< "Action " << std::to_string((int)input.action)
		<< ", control " << std::to_string((int)input.control)
		<< ", held time " << std::to_string(input.held_time);
	return out;
}

std::vector<fr::Input> findInputs(std::vector<fr::Input> inputs,
		fr::Control control) {
	std::vector<fr::Input> matches;

	for (int i = 0; i < inputs.size(); ++i) {
		if (inputs[i].control == control)
			matches.push_back(inputs[i]);
	}

	return matches;
}

std::vector<fr::Input> findInputs(std::vector<fr::Input> inputs,
		fr::Control control, fr::Action action) {
	std::vector<fr::Input> matches;

	for (int i = 0; i < inputs.size(); ++i) {
		if (inputs[i].control == control && inputs[i].action == action)
			matches.push_back(inputs[i]);
	}

	return matches;
}

void fr::updateInputs(float dt, std::vector<fr::Input> &inputs,
		std::vector<fr::Input> buffer, std::vector<int> controls) {
	std::vector<Input> new_inputs;

	for (int i = 0; i < controls.size(); ++i) {
		Control control = (Control)i;

		std::vector<Input> prev_inputs = findInputs(inputs, control);

		// inputs vector, unlike buffer, will never have two with the same
		// action so either prev_inputs is empty or has an action at 0
		bool was_down = !prev_inputs.empty()
				&& (prev_inputs[0].action == Action::press
				|| prev_inputs[0].action == Action::hold);

		bool is_down = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)controls[i]);

		if (is_down && was_down) {
			float held_time = 0;
			if (!prev_inputs.empty())
				held_time = prev_inputs[0].held_time + dt;

			new_inputs.push_back(Input{control, Action::hold, held_time});
		}

		if (!is_down && was_down)
			new_inputs.push_back(Input{control, Action::release});

		if (is_down && !was_down)
			new_inputs.push_back(Input{control, Action::press});
	}

	inputs = new_inputs;
}

// TODO Joystick
void fr::updateInputs(float dt, int joytstick, std::vector<fr::Input> &inputs,
		std::vector<fr::Input> buffer, std::vector<int> controls) {
}

// TODO Explain
void fr::updateBuffer(float dt, std::vector<fr::Input> &buffer, 
		float &buffer_ttl, std::vector<fr::Input> inputs) {
	buffer_ttl -= dt;

	if (buffer_ttl < 0) {
		buffer.clear();
		buffer_ttl = BUFFER_TTL;
	}

	for (int i = 0; i < inputs.size(); ++i) {
		buffer_ttl = BUFFER_TTL;

		if (inputs[i].action == Action::press)
			buffer.push_back(inputs[i]);
	}
}

bool fr::isBuffered(fr::Control control, std::vector<fr::Input> buffer) {
	bool buffered = control == Control::none;
	
	for (int i = 0; i < buffer.size() && !buffered; ++i)
		buffered = control == buffer[i].control;
		
	return buffered;
}
