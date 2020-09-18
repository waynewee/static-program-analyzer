#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <unordered_map>

#include <TokenType.h>

using namespace std;

#define TOKEN_TYPE_EXPR "expr"
#define TOKEN_TYPE_REL_EXPR "rel_expr"
#define TOKEN_TYPE_PUNC "punc"
#define TOKEN_TYPE_CONSTANT "constant"
#define TOKEN_TYPE_VAR "var"
#define TOKEN_TYPE_STMT "stmt"

class Token {
public:
	Token();
	Token(string tokenVal, TokenType::TOKEN_TYPE tokenType);
	
	bool isUnaryOp;
	TokenType::STMT_TYPE stmtType;
	TokenType::TOKEN_TYPE type;

	void Print();
	TokenType::STMT_TYPE GetStmtType();
	TokenType::TOKEN_TYPE GetTokenType();
	string GetTokenTypeStr(TokenType::TOKEN_TYPE tokenType);
	string GetValue();

private:
	string value;
};

#endif