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
	pause,
};

enum class Action {
	press,
	release,
	hold,
};

struct Input {
	explicit operator std::string() const;

	Control control = Control::none;
	Action action;
	float held = 0;
};

const float BUFFER_TTL = 0.15f;
const float PRESS_END = 0.15f;
const float DOUBLE_PRESS_END = 0.1f;
const Control BODY_CONTROL = Control::down;
const std::vector<Control> KONAMI_CODE{Control::up, Control::up, Control::down,
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
