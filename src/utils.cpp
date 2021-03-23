#include "utils.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

#include <toml++/toml.h>
#include "main.hpp"
#include "input.hpp"
#include "punch.hpp"

// This is mainly used to make logs more readable
void printGlobalTime() {
	std::cout << "[" << std::setw(8) << std::setfill('0') << fr::global_time
			<< "] ";
}

// Why cmath doesn't have this is beyond me
int sign(int n) {
	if (n > 0)
		return 1;
	if (n < 0)
		return -1;
	return 0;
}

std::vector<fr::Punch> getPunches(toml::array unparsed) {
	std::vector<fr::Punch> punches;

	for (int i = 0; i < unparsed.size(); ++i) {
		toml::table table = *unparsed[i].as<toml::table>();
		fr::Punch punch = parsePunch(table);
		punches.push_back(punch);
	}

	return punches;
}

fr::Punch parsePunch(toml::table punch) {
	fr::Control control = (fr::Control)(punch["control"].value_or(-1));
	bool is_lead_handed = punch["is_lead_handed"].value_or(false);
	bool is_body = punch["is_body"].value_or(false);
	int damage = punch["damage"].value_or(0);
	int body_hp_cost = punch["body_hp_cost"].value_or(0);
	float hitbox_begin = punch["hitbox_begin"].value_or(0.0f);
	float hitbox_end = punch["hitbox_end"].value_or(0.0f);
	float recovery_end = punch["recovery_end"].value_or(0.0f);
	sf::IntRect hitbox(punch["hitbox"][0].value_or(0),
			punch["hitbox"][1].value_or(0), punch["hitbox"][2].value_or(0),
			punch["hitbox"][3].value_or(0));

	return fr::Punch{control, is_lead_handed, is_body, damage, body_hp_cost,
			hitbox_begin, hitbox_end, recovery_end, hitbox};
}
