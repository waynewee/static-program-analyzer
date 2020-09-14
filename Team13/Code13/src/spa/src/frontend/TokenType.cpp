#include <string>

#include "TokenType.h"


TokenType::TOKEN_TYPE_KEYW TokenType::getTokenTypeKeyw(std::string tokenStr) {

	if (tokenStr == "procedure") {
		return TokenType::TOKEN_TYPE_KEYW::PROC;
	}

	if (tokenStr == "call") {
		return TokenType::TOKEN_TYPE_KEYW::CALL;
	}

	if (tokenStr == "read") {
		return TokenType::TOKEN_TYPE_KEYW::READ;
	}

	if (tokenStr == "while") {
		return TokenType::TOKEN_TYPE_KEYW::WHLE;
	}

	if (tokenStr == "print") {
		return TokenType::TOKEN_TYPE_KEYW::PRNT;
	}

	if (tokenStr == "if") {
		return TokenType::TOKEN_TYPE_KEYW::IF;
	}

	if (tokenStr == "then") {
		return TokenType::TOKEN_TYPE_KEYW::THEN;
	}

	if (tokenStr == "else") {
		return TokenType::TOKEN_TYPE_KEYW::ELSE;
	}

	throw "No such token type!";

}

