#pragma once

#include <string>


class Token{
public:
	enum class TOKEN_TYPE { INTR, KEYW, NAME, AROP, LGOP, PUNC };
	
	std::string value;
	TOKEN_TYPE type;

	void print();
	std::string getTokenTypeStr(TOKEN_TYPE tokenType);

	Token(std::string tokenVal, TOKEN_TYPE tokenType);
};