#include "sprite.hpp"

fr::Animation::Animation(int frames, bool loops)
		: frames(frames), loops(loops) {
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

int fr::Animation::getFrames() const {
	return frames;
}

int fr::Animation::getFrame() const {
	return frame;
}
