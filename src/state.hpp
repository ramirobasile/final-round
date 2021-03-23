#pragma once

#include <vector>

#include "input.hpp"
#include "punch.hpp"

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
		Punch punch;
		float punch_progress;

		friend std::ostream& operator<<(std::ostream &out, const State &state);
    	bool operator==(const State &state) const;
    	bool operator!=(const State &state) const { return !operator==(state); };
		void update(std::vector<Input> inputs, std::vector<Input> buffer,
				std::vector<Punch> punches);
    	bool isPunching() const;
    	bool isLeadFree() const;
    	bool isRearFree() const;
    	bool isHeadGuardUp(std::vector<Input> buffer) const;
    	bool isBodyGuardUp(std::vector<Input> buffer) const;
    	
    	private:
		void onHold(Input input, std::vector<Input> buffer,
				std::vector<Punch> punches);
		void onPress(Input input, std::vector<Input> buffer,
				std::vector<Punch> punches);
		void onRelease(Input input, std::vector<Input> buffer,
				std::vector<Punch> punches);
	};
}
