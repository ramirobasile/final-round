#pragma once

#include <vector>

#include "input.hpp"
#include "punch.hpp"

namespace fr {

enum class Movements {
	idle,
	walk_l,
	walk_r,
	stun,
};

enum class Punches {
	none,
	jab_body,
	jab_head,
	cross_body,
	cross_head,
	hook_body,
	hook_head,
	upper_body,
	upper_head,
};

enum class Dodges {
	none,
	pull,
	slip,
	duck,
	escape,
};

enum class Guards {
	none,
	head,
	body,
};

class State {
	public:
	State();
	State(std::vector<Punch> punches);
	
	void update(std::vector<Input> inputs, std::vector<Input> buffer,
			float dt);
	Punch &getPunch();
	bool isPunching();
	
	Movements movement = Movements::idle;
	Punches punch = Punches::none;
	Dodges dodge = Dodges::none;
	Guards guard = Guards::none;

	private:
	void onHold(Input input, std::vector<Input> buffer);
	void onPress(Input input, std::vector<Input> buffer,
			std::vector<Punch> punches);
	void onRelease(Input input, std::vector<Input> buffer);
	
	std::vector<Punch> punches;
	float stun;
	float tt_dodge;
	float dodge_recovery;
	float guard_recovery;
};

} // namespace fr
