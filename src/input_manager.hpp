#pragma once

#include <vector>

#include "input.hpp"
#include "punch.hpp"

namespace fr {

class InputManager {
	public:
	InputManager() {}; // Empty
	InputManager(Device device, std::vector<int> controls, int joystick = -1);

	void update(float dt);
	bool inputted(Control control, Action action = Action::none,
			float held = -1) const;
	bool inputted(Input input) const;
	bool buffered(Control control, Action action = Action::none,
			float held = -1) const;

	private:
	void updateInputs(float dt);
	void updateBuffer(float dt);

	Device device = Device::keyboard;
	std::vector<int> controls;
	int joystick = -1;
	std::vector<Input> inputs;
	std::vector<Input> buffer;
	float buffer_ttl = 0;
};

} // namespace fr
