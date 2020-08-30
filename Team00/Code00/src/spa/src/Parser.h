#pragma once

#include <string>
#include <map>
#include <vector>

#include "Token.h"

class Parser {
public:
	std::map<std::string, std::string> cache;
	std::string text;
	int pos;
	int len;
	std::string token;
	std::vector<std::string> keywords = { "procedure", "print", "read", "call", "while", "if", "then", "else" };
	std::vector<Token> tokenList;

	int parse();
	
	void resetToken();
	void printToken(std::string type);
	void appendStrToToken(std::string str);
	void appendCharToToken(char c);
	void addToken(Token::TOKEN_TYPE tokenType);
	void printTokenList();

	bool isWhiteSpace(char c);
	bool isPunc(char c);
	bool isArop(char c);
	bool isLgopPart(char c);
	bool isLgop(std::string str);

	Parser(std::string input);
};