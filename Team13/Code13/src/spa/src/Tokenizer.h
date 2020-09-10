#pragma once

#include <string>
#include <map>
#include <vector>

#include "Token.h"
#include "TokenType.h"

class Tokenizer {
public:
	std::map<std::string, std::string> cache;
	std::string text;
	int pos;
	int len;
	std::string tokenStr;
	std::vector<std::string> keywords = { "procedure", "print", "read", "call", "while", "if", "then", "else" };
	std::vector<Token> tokenList;

	int tokenize();

	void resetTokenStr();
	void printToken(std::string type);
	void appendStrToTokenStr(std::string str);
	void appendCharToTokenStr(char c);
	void testAndSetUnary(Token* currPtr, Token prev);
	Token* addToken(TokenType::TOKEN_TYPE tokenType);
	void printTokenList();
	std::vector<Token> getTokenList();

	bool isWhiteSpace(char c);
	bool isPunc(char c);
	bool isArop(char c);
	bool isLgopPart(char c);
	bool isLgop(std::string str);

	Tokenizer(std::string input);
};