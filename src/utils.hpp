#pragma once

#include <toml++/toml.h>
#include "punch.hpp"

void printGlobalTime();
int sign(int n);
std::vector<fr::Punch> getPunches(toml::array unparsed);
fr::Punch parsePunch(toml::table punch);
