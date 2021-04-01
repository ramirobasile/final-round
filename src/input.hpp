#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace fr {

enum class Device {
	keyboard,
	joystick,
};

enum class Control {
	none = -1,
	left,
	right,
	up,
	down,
	jab,
	power,
	guard,
	pause,
};

enum class Action {
	press,
	double_press,
	long_press,
	release,
	hold,
};

struct Input {
	Control control = Control::none;
	Action action;
	float held_time = 0;

	explicit operator std::string() const;
};

const float BUFFER_TTL = 0.15f;
const float LONG_PRESS_T = 0.066f;
const float DOUBLE_PRESS_T = 0.05f;
const Control BODY_CONTROL = Control::down;
const std::vector<Control> KONAMI{Control::up, Control::up, Control::down,
		Control::down, Control::down, Control::left, Control::right,
		Control::left, Control::right, Control::power, Control::jab};

void updateInputs(std::vector<Input> &inputs, std::vector<Input> buffer,
		float buffer_ttl, std::vector<int> controls);
void updateInputs(int joytstick, std::vector<Input> &inputs,
		std::vector<Input> buffer, float buffer_ttl, std::vector<int> controls);
void updateBuffer(std::vector<Input> &buffer, float &clear_time,
		std::vector<Input> inputs);
bool buffered(Control control, std::vector<Input> buffer);

} // namespace fr
