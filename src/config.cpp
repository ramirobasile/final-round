#include "config.hpp"

#include <fstream>
#include <map>
#include <sstream>
#include <string>

void fr::ConfigFile::parse(std::ifstream file) {
	cfg.clear();
	std::string last_section;

	for(std::string line; std::getline(file, line);) {
		if (line.empty() || line.front() == '\n' || line.front() == COMMENT)
			continue;

		if (line.front() == OPEN_SECTION && line.back() == CLOSE_SECTION) {
			std::string section = line.substr(1, line.size() - 2);
			cfg[section];
			last_section = section;
		} else {
			std::string key = line.substr(0, line.find(DELIMIT));
			std::string value = line.substr(line.find(DELIMIT) + 1);
			cfg[last_section][key] = value;
		}
	}
}

bool fr::ConfigFile::hasKey(std::string section, std::string key) {
	return cfg.find(section) != cfg.end()
			&& cfg[section].find(key) != cfg[section].end();
}

int fr::ConfigFile::getInt(std::string section, std::string key, int fallback) {
	int value = fallback;

	if (hasKey(section, key))
		value = std::stoi(cfg[section][key]);

	return value;
}

float fr::ConfigFile::getFloat(std::string section, std::string key,
		float fallback) {
	float value = fallback;

	if (hasKey(section, key))
		value = std::stof(cfg[section][key]);

	return value;
}

std::string fr::ConfigFile::getStr(std::string section, std::string key,
		std::string fallback) {
	std::string value = fallback;

	if (hasKey(section, key))
		value = cfg[section][key];

	return value;
}

bool fr::ConfigFile::getBool(std::string section, std::string key,
		bool fallback) {
	bool value = fallback;

	if (hasKey(section, key))
		value = cfg[section][key] == "true";

	return value;
}
