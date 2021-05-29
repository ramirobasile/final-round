#pragma once

#include "animation.hpp"

namespace fr {

class Dodge {
	public:
	Dodge() {}; // Empty constructor
	Dodge(int cost, float active_begin, float active_end, float recovery_end,
			int min_distance, Animation animation);

	void update(float dt);
	void reset();
	void interrupt();
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;
	float getCost() const;
	int getMinDistance() const;
	
	Animation animation;
	
	private:
	int cost = -1;
	float active_begin = -1;
	float active_end = -1;
	float recovery_end = -1;
	int min_distance = -1;
	float progress = -1;
};

} // namespace fr
