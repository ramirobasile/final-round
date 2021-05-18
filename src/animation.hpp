#pragma once

namespace fr {

class Animation {
	public:
	Animation() {}; // Empty constructor
	Animation(int sheet_y, int frames, bool loops);
	
	bool operator==(const Animation other) const;
	void nextFrame();
	void reset();
	int getSheetY() const;
	int getFrames() const;
	int getFrame() const;
	
	private:
	int sheet_y = -1;
	int frames = -1;
	int frame = 0;
	bool loops = false;
};

} // namespace fr
