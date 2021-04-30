#include "input.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"

fr::Input::operator std::string() const {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << held;

	return std::to_string((int)control) + " " + std::to_string((int)action)
			+ " " + stream.str();
}

void fr::updateInputs(std::vector<fr::Input> &inputs,
		std::vector<fr::Input> buffer, float buffer_ttl,
		std::vector<int> controls, float dt, int joystick) {
	std::vector<Input> new_inputs;

	for (int i = 0; i < controls.size(); ++i) {
		Control control = (Control)i;

		bool is_down;
		if (joystick == -1) {
			sf::Keyboard::Key key = (sf::Keyboard::Key)controls[(int)control];
			is_down = sf::Keyboard::isKeyPressed(key);
		} else { 
			int button = controls[(int)control];
			is_down = sf::Joystick::isButtonPressed(joystick, button);
		}

		Input prev;
		for (int j = 0; j < inputs.size() && prev.control != control; ++j) {
			if (inputs[j].control == control)
				prev = inputs[j];
		}
		bool was_down = prev.control == control && prev.action != Action::release;

		float held = prev.held + dt;
		
		bool pressed_before = !buffer.empty() && buffer.back().control == control
				&& buffer.back().action == Action::release;
		bool within_dp_end = BUFFER_TTL - buffer_ttl < DOUBLE_PRESS_END;

		if (is_down && was_down)
			new_inputs.push_back(Input{control, Action::hold, held});

		if (!is_down && was_down)
			new_inputs.push_back(Input{control, Action::release, held});

		if (is_down && !was_down)
			new_inputs.push_back(Input{control, Action::press, held});
			
		if (is_down && !was_down && pressed_before && within_dp_end)
			new_inputs.push_back(Input{control, Action::double_press, held});
	}

	inputs = new_inputs;
}

// TODO Explain
void fr::updateBuffer(std::vector<fr::Input> &buffer,
		float &buffer_ttl, std::vector<fr::Input> inputs, float dt) {
	buffer_ttl -= dt;

	if (buffer_ttl < 0) {
		buffer.clear();
		buffer_ttl = BUFFER_TTL;
	}

	for (int i = 0; i < inputs.size(); ++i) {
		buffer_ttl = BUFFER_TTL;

		buffer.push_back(inputs[i]);
	}
}

bool fr::inputted(fr::Control control, std::vector<fr::Input> inputs) {
	bool res = control == Control::none;
	
	for (int i = 0; i < inputs.size() && !res; ++i)
		res = control == inputs[i].control;
		
	return res;
}
