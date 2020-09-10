#include <string>
#include <iostream>

#include "Token.h"
#include "TokenType.h"

Token::Token(std::string tokenVal, TokenType::TOKEN_TYPE tokenType) {
	value = tokenVal;
	type = tokenType;
	isUnaryOp = false;
}

Token::Token(){}

void Token::print() {

	std::string output = "";

	output += getTokenTypeStr(type) + "\t" + value;

	std::cout << output << std::endl;
}

std::string Token::getValue() {
	return value;
}

TokenType::TOKEN_TYPE Token::getTokenType() {
	return type;
}


TokenType::TOKEN_TYPE_KEYW Token::getKeywordType() {
	return keywordType;
}

std::string Token::getTokenTypeStr(TokenType::TOKEN_TYPE tokenType) {

	switch (tokenType) {
	case TokenType::TOKEN_TYPE::AROP:
		return "AROP";
	case TokenType::TOKEN_TYPE::LGOP:
		return "LGOP";
	case TokenType::TOKEN_TYPE::PUNC:
		return "PUNC";
	case TokenType::TOKEN_TYPE::INTR:
		return "INTR";
	case TokenType::TOKEN_TYPE::NAME:
		return "NAME";
	case TokenType::TOKEN_TYPE::KEYW:
		return "KEYW";
	default:
		return "";
	}

}