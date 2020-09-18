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

void Token::print() {

	string output = "";

	output += getTokenTypeStr(type) + "\t" + value;

	cout << output << endl;
}

string Token::getValue() {
	return value;
}

TokenType::TOKEN_TYPE Token::getTokenType() {
	return type;
}


TokenType::STMT_TYPE Token::getStmtType() {
	return stmtType;
}

string Token::getTokenTypeStr(TokenType::TOKEN_TYPE tokenType) {

	switch (tokenType) {
	case TokenType::TOKEN_TYPE::expr:
		return "expr";
	case TokenType::TOKEN_TYPE::rel_expr:
		return "rel_expr";
	case TokenType::TOKEN_TYPE::punc:
		return "punc";
	case TokenType::TOKEN_TYPE::constant:
		return "constant";
	case TokenType::TOKEN_TYPE::var:
		return "var";
	case TokenType::TOKEN_TYPE::stmt:
		return "stmt";
	default:
		return "";
	}

}