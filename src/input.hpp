#pragma once

#include <iostream>
#include <string>
#include <vector>

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
	a,
	b,
	pause,
};

enum class Action {
	press,
	release,
	hold,
	double_press,
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
		Control::left, Control::right, Control::b, Control::a};

void updateInputs(std::vector<Input> &inputs, std::vector<Input> buffer,
		float buffer_ttl, std::vector<int> controls, float dt, 
		int joystick = -1);
void updateBuffer(std::vector<Input> &buffer, float &clear_time,
		std::vector<Input> inputs, float dt);
bool inputted(Control control, std::vector<Input> inputs);

} // namespace fr
