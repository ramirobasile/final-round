#pragma once

#include <vector>

#include "input.hpp"
#include "punch.hpp"

namespace fr {

enum class Movement {
	idle,
	walk_l,
	walk_r,
	weave,
	pull,
	duck,
	stun,
	escape,
};

class State {
	public:
	Movement movement = Movement::idle;
	Punch punch = Punch();
	bool guard_high = false;
	bool guard_low = false;

	void update(std::vector<Input> inputs, std::vector<Input> buffer,
			std::vector<Punch> punches, float dt);
	bool isPunching();
	bool isGuarding();

	private:
	void onHold(Input input, std::vector<Input> buffer);
	void onPress(Input input, std::vector<Input> buffer,
			std::vector<Punch> punches);
	void onRelease(Input input, std::vector<Input> buffer);
};

} // namespace fr
