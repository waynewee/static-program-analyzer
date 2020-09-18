#include <iostream>
#include <string>

#include <Token.h>
#include <TokenType.h>

using namespace std;

Token::Token(string tokenVal, TokenType::TOKEN_TYPE tokenType) {
	value = tokenVal;
	type = tokenType;
	isUnaryOp = false;
}

Token::Token(){}

void Token::Print() {

	string output = "";

	output += GetTokenTypeStr(type) + "\t" + value;

	cout << output << endl;
}

string Token::GetValue() {
	return value;
}

TokenType::TOKEN_TYPE Token::GetTokenType() {
	return type;
}


TokenType::STMT_TYPE Token::GetStmtType() {
	return stmtType;
}

string Token::GetTokenTypeStr(TokenType::TOKEN_TYPE tokenType) {

	switch (tokenType) {
	case TokenType::TOKEN_TYPE::expr:
		return TOKEN_TYPE_EXPR;
	case TokenType::TOKEN_TYPE::rel_expr:
		return TOKEN_TYPE_REL_EXPR;
	case TokenType::TOKEN_TYPE::punc:
		return TOKEN_TYPE_PUNC;
	case TokenType::TOKEN_TYPE::constant:
		return TOKEN_TYPE_CONSTANT;
	case TokenType::TOKEN_TYPE::var:
		return TOKEN_TYPE_VAR;
	case TokenType::TOKEN_TYPE::stmt:
		return TOKEN_TYPE_STMT;
	default:
		return "";
	}

}