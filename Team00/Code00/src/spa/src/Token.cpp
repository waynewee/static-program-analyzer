#include <string>

#include <Token.h>

Token::Token(std::string tokenVal, std::string tokenType) {
	value = tokenVal;
	type = tokenType;
}