#pragma once

namespace fr {
	struct Animation {
		int column;
		int frame;
		float fps;
		bool repeat;
		bool resume;
		float width;
		float height;

		float time;
	};
}
