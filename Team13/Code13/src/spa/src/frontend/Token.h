#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <unordered_map>

#include "TokenType.h"

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
	Token(string token_val, TokenType::TOKEN_TYPE token_type);
	
	bool is_unary_op_;
	TokenType::STMT_TYPE stmt_type_;
	TokenType::TOKEN_TYPE type_;

	void Print();
	TokenType::STMT_TYPE GetStmtType();
	TokenType::TOKEN_TYPE GetTokenType();
	string GetTokenTypeStr(TokenType::TOKEN_TYPE token_type);
	string GetValue();

private:
	string value_;
};

#endif