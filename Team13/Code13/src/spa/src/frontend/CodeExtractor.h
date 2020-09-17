#pragma once
#include <vector>

#include "Token.h"

class CodeExtractor {
public:
	std::string filename;
	CodeExtractor(std::string file_n);
	std::string extract();
};