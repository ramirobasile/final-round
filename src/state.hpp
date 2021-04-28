#pragma once

#include <vector>

#include "dodge.hpp"
#include "input.hpp"
#include "punch.hpp"

namespace fr {

enum class Movements {
	idle,
	walk_l,
	walk_r,
	stun,
};

enum class Guards {
	none,
	head,
	body,
};

class State {
	public:
	State();
	State(std::vector<Punch> punches, std::vector<Dodge> dodges);
	
	void update(std::vector<Input> inputs, std::vector<Input> buffer,
			float dt);

	Punch punch;
	Dodge dodge;
	Movements movement = Movements::idle;
	Guards guard = Guards::none;

	private:
	std::vector<Punch> punches;
	std::vector<Dodge> dodges;
	float stun;
	float guard_recovery;
};

} // namespace fr
