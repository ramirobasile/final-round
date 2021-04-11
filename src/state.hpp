#pragma once

#include <vector>

#include "input.hpp"
#include "punch.hpp"

namespace fr {

enum class Movements {
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
	State();
	State(std::vector<Punch> punches);
	
	void update(std::vector<Input> inputs, std::vector<Input> buffer,
			float dt);
	Punch &getPunch();
	bool isPunching();
	bool isGuarding() const;
	
	Movements movement = Movements::idle;
	Punches punch = Punches::none;
	bool guard_high = false;
	bool guard_low = false;

	private:
	void onHold(Input input, std::vector<Input> buffer);
	void onPress(Input input, std::vector<Input> buffer,
			std::vector<Punch> punches);
	void onRelease(Input input, std::vector<Input> buffer);
	
	std::vector<Punch> punches;
	float tt_dodge;
	float dodge;
	float dodge_recovery;
	float guard_recovery;
	float stun;
};

} // namespace fr
