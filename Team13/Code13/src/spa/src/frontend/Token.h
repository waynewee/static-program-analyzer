#pragma once

#include <unordered_map>
#include <string>

#include "TokenType.h"

class Token {
public:

	TokenType::TOKEN_TYPE type;
	TokenType::STMT_TYPE stmtType;

	bool isUnaryOp;

	void print();
	std::string getTokenTypeStr(TokenType::TOKEN_TYPE tokenType);
	TokenType::TOKEN_TYPE getTokenType();
	TokenType::STMT_TYPE getStmtType();

	Token(std::string tokenVal, TokenType::TOKEN_TYPE tokenType);
	Token();

	std::string getValue();

private:
	std::string value;
};