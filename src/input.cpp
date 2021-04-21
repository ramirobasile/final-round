#include "input.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"

fr::Input::operator std::string() const {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << held;

	return std::to_string((int)control) + " " + std::to_string((int)action)
			+ " " + stream.str();
}

void fr::updateInputs(std::vector<fr::Input> &inputs, std::vector<fr::Input> buffer,
		float buffer_ttl, std::vector<int> controls, float dt) {
	std::vector<Input> new_inputs;

	for (int i = 0; i < controls.size(); ++i) {
		Controls control = (Controls)i;

		Input prev;
		for (int i = 0; i < inputs.size() && prev.control != control; ++i) {
			if (inputs[i].control == control)
				prev = inputs[i];
		}
		bool was_down = prev.control == control && prev.action != Actions::release;
		bool is_down = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)controls[i]);
		float held = prev.held + dt;

		if (is_down && was_down)
			new_inputs.push_back(Input{control, Actions::hold, held});

		if (!is_down && was_down)
			new_inputs.push_back(Input{control, Actions::release, held});

		if (is_down && !was_down)
			new_inputs.push_back(Input{control, Actions::press, held});
			
		bool pressed_before = !buffer.empty() && buffer.back().control == control
				&& buffer.back().action == Actions::release;
		bool within_dp_end = BUFFER_TTL - buffer_ttl < DOUBLE_PRESS_END;
		if (is_down && !was_down && pressed_before && within_dp_end)
			new_inputs.push_back(Input{control, Actions::double_press, held});
	}

	inputs = new_inputs;
}

// TODO Joystick
void fr::updateInputs(int joytstick, std::vector<fr::Input> &inputs,
		std::vector<fr::Input> buffer, float buffer_ttl,
		std::vector<int> controls, float dt) {
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

bool fr::inputted(fr::Controls control, std::vector<fr::Input> inputs) {
	bool res = control == Controls::none;
	
	for (int i = 0; i < inputs.size() && !res; ++i)
		res = control == inputs[i].control;
		
	return res;
}
