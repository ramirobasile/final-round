#pragma once

#include <iostream>
#include <vector>

#include "input.hpp"
#include "punch.hpp"
#include "physics.hpp"

namespace fr {
	enum class Movement {
		idle,
		walk_b,
		walk_f,
		dash_b,
		dash_f,
		punch,
		weave,
		pull,
		duck,
		stun,
	};

	class State {
		public:
		Movement movement = Movement::idle;
		int punch;
		float punch_progress;

		friend std::ostream& operator<<(std::ostream &out, const State &state);
    	bool operator==(const State &state) const;
    	bool operator!=(const State &state) const { return !operator==(state); };
    	Punch getPunch() const;
    	bool isPunching() const;
    	bool isLeadFree() const;
    	bool isRearFree() const;
    	bool isBodyOpen() const;
		void update(float dt, std::vector<Input> inputs, 
				std::vector<Input> buffer);
    	
    	private:
		void onHold(Input input, std::vector<Input> buffer);
		void onPress(Input input, std::vector<Input> buffer);
		void onRelease(Input input, std::vector<Input> buffer);
	};
}
