#include "input.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "main.hpp"

fr::Input::operator std::string() const {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << held_time;

	return "Control " + std::to_string((int)control)
			+ ", action " + std::to_string((int)action)
			+ ", held for " + stream.str() + " seconds";
}

void fr::updateInputs(std::vector<fr::Input> &inputs, std::vector<fr::Input> buffer,
		float buffer_ttl, std::vector<int> controls) {
	std::vector<Input> new_inputs;

	for (int i = 0; i < controls.size(); ++i) {
		Control control = (Control)i;

		Input prev;
		for (int i = 0; i < inputs.size() && prev.control != control; ++i) {
			if (inputs[i].control == control)
				prev = inputs[i];
		}

		bool is_down = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)controls[i]);
		bool was_down = prev.control == control
				&& (prev.action == Action::press || prev.action == Action::hold);

		bool press_buffered = !buffer.empty() && buffer.back().control == control
				&& buffer.back().action == Action::release;
		bool in_dp_window = BUFFER_TTL - buffer_ttl < DOUBLE_PRESS_T;

		float held_time = prev.held_time + dt;

		if (is_down && was_down)
			new_inputs.push_back(Input{control, Action::hold, held_time});

		if (!is_down && was_down)
			new_inputs.push_back(Input{control, Action::release});

		if (is_down && !was_down) {
			if (press_buffered && in_dp_window)
				new_inputs.push_back(Input{control, Action::double_press});
			else
				new_inputs.push_back(Input{control, Action::press});
		}

	}

	inputs = new_inputs;
}

// TODO Joystick
void fr::updateInputs(int joytstick, std::vector<fr::Input> &inputs,
		std::vector<fr::Input> buffer, float buffer_ttl, std::vector<int> controls) {
}

// TODO Explain
void fr::updateBuffer(std::vector<fr::Input> &buffer,
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

bool fr::buffered(fr::Control control, std::vector<fr::Input> buffer) {
	bool res = control == Control::none;
	
	for (int i = 0; i < buffer.size() && !res; ++i)
		res = control == buffer[i].control;
		
	return res;
}
