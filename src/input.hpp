#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace fr {

enum class Devices {
	keyboard,
	joystick,
};

enum class Controls {
	none = -1,
	left,
	right,
	up,
	down,
	a,
	b,
	pause,
};

enum class Actions {
	press,
	release,
	hold,
	double_press,
};

struct Input {
	explicit operator std::string() const;

	Controls control = Controls::none;
	Actions action;
	float held = 0;
};

const float BUFFER_TTL = 0.15f;
const float PRESS_END = 0.15f;
const float DOUBLE_PRESS_END = 0.1f;
const Controls BODY_CONTROL = Controls::down;
const std::vector<Controls> KONAMI_CODE{Controls::up, Controls::up, Controls::down,
		Controls::down, Controls::down, Controls::left, Controls::right,
		Controls::left, Controls::right, Controls::b, Controls::a};

void updateInputs(std::vector<Input> &inputs, std::vector<Input> buffer,
		float buffer_ttl, std::vector<int> controls, float dt);
void updateInputs(int joytstick, std::vector<Input> &inputs,
		std::vector<Input> buffer, float buffer_ttl, std::vector<int> controls,
		float dt);
void updateBuffer(std::vector<Input> &buffer, float &clear_time,
		std::vector<Input> inputs, float dt);
bool inputted(Controls control, std::vector<Input> inputs);

} // namespace fr
