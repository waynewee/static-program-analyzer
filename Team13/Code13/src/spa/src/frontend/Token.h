#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <unordered_map>

#include <TokenType.h>

using namespace std;

class Token {
public:

	TokenType::TOKEN_TYPE type;
	TokenType::STMT_TYPE stmtType;

	bool isUnaryOp;

	void print();
	string getTokenTypeStr(TokenType::TOKEN_TYPE tokenType);
	TokenType::TOKEN_TYPE getTokenType();
	TokenType::STMT_TYPE getStmtType();

	Token(string tokenVal, TokenType::TOKEN_TYPE tokenType);
	Token();

	string getValue();

private:
	string value;
};

#endif