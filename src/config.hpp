#pragma once

#include <fstream>
#include <map>
#include <string>

namespace fr {

class ConfigFile {
	public:
	void parse(std::ifstream file);
	bool hasKey(std::string section, std::string key);
	int getInt(std::string section, std::string key, int fallback = -1);
	float getFloat(std::string section, std::string key,
			float fallback = -1.0f);
	std::string getStr(std::string section, std::string key,
			std::string fallback = "");
	bool getBool(std::string section, std::string key,
			bool fallback = false);

	private:
	const char OPEN_SECTION = '[';
	const char CLOSE_SECTION = ']';
	const char DELIMIT = '=';
	const char COMMENT = ';';

	std::map<std::string, std::map<std::string, std::string>> cfg;
};

} // namespace fr
