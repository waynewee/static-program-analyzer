#pragma once

#include <string>


class Token{
public:
	std::string value;
	std::string type;

	Token(std::string tokenVal, std::string tokenType);
};