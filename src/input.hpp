#pragma once

#include <string>
#include <vector>

namespace fr {

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
	jab,		// A
	cross,		// B
	upper,		// X
	hook,		// Y
	dodge,		// L
	body,		// R
	pause,		// Start
};

enum class Action {
	none = -1,
	press,
	release,
	hold,
	double_press,
};

struct Input {
	explicit operator std::string() const;

	Control control = Control::none;
	Action action = Action::none;
	float held = 0;
};

const float BUFFER_TTL = 0.15f;
const float DOUBLE_PRESS_END = 0.1f;
const std::vector<Control> KONAMI_CODE{Control::up, Control::up, Control::down,
		Control::down, Control::down, Control::backwards, Control::forwards,
		Control::backwards, Control::forwards, Control::cross,
		Control::jab};

} // namespace fr
