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

	friend std::ostream& operator<<(std::ostream &out, const State &state);
	bool operator==(const State &state) const;
	bool operator!=(const State &state) const { return !operator==(state); };

	void update(std::vector<Input> inputs, std::vector<Input> buffer,
			std::vector<Punch> punches);
	bool punching();

	private:
	void onHold(Input input, std::vector<Input> buffer);
	void onPress(Input input, std::vector<Input> buffer,
			std::vector<Punch> punches);
	void onRelease(Input input, std::vector<Input> buffer);
};

} // namespace fr
