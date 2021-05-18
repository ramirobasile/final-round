#include "input_manager.hpp"

#include <vector>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "input.hpp"
#include "punch.hpp"

fr::InputManager::InputManager(fr::Device device, std::vector<int> controls,
		int joystick)
		: device(device), controls(controls), joystick(joystick) {
}

void fr::InputManager::update(float dt) {
	updateInputs(dt);
	updateBuffer(dt);
}

void fr::InputManager::updateInputs(float dt) {
	std::vector<Input> new_inputs;

	for (int i = 0; i < controls.size(); ++i) {
		Control control = (Control)i;

		bool is_down = false;
		if (device == Device::keyboard) {
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

void fr::InputManager::updateBuffer(float dt) {
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

bool fr::InputManager::inputted(fr::Control control, fr::Action action,
		float held) const {
	bool match = control == Control::none;
	for (int i = 0; i < inputs.size() && !match; ++i) {
		match = control == inputs[i].control
				&& (action == Action::none || action == inputs[i].action)
				&& (held == -1 || held <= inputs[i].held);
	}

	return match;
}

bool fr::InputManager::inputted(fr::Input input) const {
	return inputted(input.control, input.action, input.held);
}

bool fr::InputManager::buffered(fr::Control control, fr::Action action,
		float held) const {
	bool match = control == Control::none;
	for (int i = 0; i < buffer.size() && !match; ++i) {
		match = control == buffer[i].control
				&& (action == Action::none || action == buffer[i].action)
				&& (held == -1 || held <= buffer[i].held);
	}

	return match;
}
