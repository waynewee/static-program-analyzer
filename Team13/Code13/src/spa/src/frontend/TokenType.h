#pragma once

#include <string>

class TokenType{
public:
	enum class TOKEN_TYPE { KEYW, PUNC, LGOP, AROP, INTR, NAME, PROC };

	enum class TOKEN_TYPE_KEYW { PROC, CALL, READ, WHLE, PRNT, IF, THEN, ELSE };

	static TOKEN_TYPE_KEYW getTokenTypeKeyw(std::string tokenStr);
};