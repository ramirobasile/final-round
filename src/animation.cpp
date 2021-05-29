#include "sprite.hpp"

fr::Animation::Animation(int sheet_y, int frames, bool loops)
		: sheet_y(sheet_y), frames(frames), loops(loops) {
}

void fr::Animation::nextFrame() {
	if (frame < frames - 1)
		frame++;
	else if (loops)
		frame = 0;
}

void fr::Animation::reset() {
	frame = 0;
}

int fr::Animation::getSheetY() const {
	return sheet_y;
}

int fr::Animation::getFrames() const {
	return frames;
}

int fr::Animation::getFrame() const {
	return frame;
}
