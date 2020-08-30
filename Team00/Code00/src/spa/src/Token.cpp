#include <string>
#include <iostream>

#include <Token.h>

Token::Token(std::string tokenVal, Token::TOKEN_TYPE tokenType) {
	value = tokenVal;
	type = tokenType;
}

void Token::print() {

	std::string output = "";

	output += getTokenTypeStr(type) + "\t" + value;

	std::cout << output << std::endl;
}

std::string Token::getTokenTypeStr(TOKEN_TYPE tokenType) {

	switch (tokenType) {
	case TOKEN_TYPE::AROP:
		return "AROP";
	case TOKEN_TYPE::LGOP:
		return "LGOP";
	case TOKEN_TYPE::PUNC:
		return "PUNC";
	case TOKEN_TYPE::INTR:
		return "INTR";
	case TOKEN_TYPE::NAME:
		return "NAME";
	case TOKEN_TYPE::KEYW:
		return "KEYW";
	default:
		return "";
	}

}