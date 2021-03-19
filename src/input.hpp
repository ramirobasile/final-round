#pragma once

#include <iostream>
#include <vector>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"

namespace fr {
	const float BUFFER_TTL = 0.15f;

	enum class Device {
		keyboard,
		joystick,
	};

	enum class Control {
		none = -1,
		backwards,
		forwards,
		up,
		down,
		jab,
		cross,
		hook,
		upper,
		dodge,
		start,
	};

	enum class Action {
		press,
		release,
		hold,
	};

	struct Input {
		Control control;
		Action action;

		float held_time = 0;

		friend std::ostream& operator<<(std::ostream &out, const Input &input);
	};

	void updateInputs(float dt, std::vector<Input> &inputs, 
			std::vector<Input> buffer, std::vector<int> controls);
	void updateInputs(float dt, int joytstick, std::vector<Input> &inputs,
			std::vector<Input> buffer, std::vector<int> controls);
	void updateBuffer(float dt, std::vector<Input> &buffer, float &clear_time,
			std::vector<Input> inputs);
	bool isBuffered(Control control, std::vector<Input> buffer);
}
