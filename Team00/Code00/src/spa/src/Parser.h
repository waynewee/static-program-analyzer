#pragma once

#include <string>
#include <map>
#include <vector>

#include "Token.h"

class Parser {
public:
	int parse(std::string input);

	Parser();
};