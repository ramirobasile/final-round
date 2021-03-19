#include "stats.hpp"

fr::Stats::Stats(int punches, int body, int feet)
		: punches(punches), body(body), feet(feet) {
	max_body_hp += 5 * body;
	walk_speed += 2 * feet;
	tt_weave -= 0.02f * feet;
	tt_pull -= 0.02f * feet;
	tt_duck -= 0.02f * feet;
}
