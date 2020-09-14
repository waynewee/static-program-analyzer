#include <fstream>
#include <iostream>

#include "CodeExtractor.h"

CodeExtractor::CodeExtractor(std::string file_n) {
	filename = file_n;
}

std::string CodeExtractor::extract() {

	std::ifstream ifs(filename);
	std::string input;

	input.assign((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	return input;
}