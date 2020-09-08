#pragma once

#include <string>

#include "TokenType.h"

class Token{
public:

	TokenType::TOKEN_TYPE type;
	TokenType::TOKEN_TYPE_KEYW keywordType;

	void print();
	std::string getTokenTypeStr(TokenType::TOKEN_TYPE tokenType);
	TokenType::TOKEN_TYPE getTokenType();
	TokenType::TOKEN_TYPE_KEYW getKeywordType();

	Token(std::string tokenVal, TokenType::TOKEN_TYPE tokenType);
	Token();

	std::string getValue();

private:
	std::string value;
};