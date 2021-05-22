#pragma once

namespace fr {

class Animation {
	public:
	Animation() {}; // Empty constructor
	Animation(int frames, bool loops);
	
	enum class Name {
		idle,
		idle_guard_head,
		idle_guard_body,
		walk,
		walk_guard_head,
		walk_guard_body,
		pull,
		slip,
		duck,
		hit_head,
		hit_body,
		ko,
		jab,
		jab_body,
		cross,
		cross_body,
		upper,
		upper_body,
		hook,
		hook_body,
	};
	
	void nextFrame();
	void reset();
	int getFrames() const;
	int getFrame() const;
	
	private:
	int frames = -1;
	int frame = 0;
	bool loops = false;
};

} // namespace fr
