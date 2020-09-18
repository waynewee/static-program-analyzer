#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <map>
#include <vector>

#include <Token.h>
#include <TokenType.h>

using namespace std;

class Tokenizer {
public:
	map<string, string> cache;
	string text;
	int pos;
	int len;
	string tokenStr;
	vector<string> stmtNames = { "procedure", "print", "read", "call", "while", "if", "then", "else" };
	vector<Token> tokenList;

	int tokenize();

	void resetTokenStr();
	void printToken(string type);
	void appendStrToTokenStr(string str);
	void appendCharToTokenStr(char c);
	void testAndSetUnary(Token* currPtr, Token prev);
	Token* addToken(TokenType::TOKEN_TYPE tokenType);
	void printTokenList();
	vector<Token> getTokenList();

	bool isWhiteSpace(char c);
	bool isPunc(char c);
	bool isArop(char c);
	bool isLgopPart(char c);
	bool isLgop(string str);

	Tokenizer(string input);
};

#endif