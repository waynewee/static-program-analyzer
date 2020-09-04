#pragma once
#include <vector>

#include "Token.h"
#include "TNode.h"
#include "Statement.h"

class Parser {
public:
	Parser();
	std::vector<Statement> statementList;
	Statement currentStatement;
	int parse(std::string input);

private:
	TNode parseProgram(std::vector<Token> tokenList);
	TNode parseStatement(int* tokenIndex, std::vector<Token> tokenList); 

	TNode parseProcStatement(int* tokenIndex, std::vector<Token> tokenList);
	TNode parseReadStatement(int* tokenIndex, std::vector<Token> tokenList);
	TNode parsePrntStatement(int* tokenIndex, std::vector<Token> tokenList);
	TNode parseIfStatement(int* tokenIndex, std::vector<Token> tokenList);
	TNode parseWhleStatement(int* tokenIndex, std::vector<Token> tokenList);
	TNode parseCallStatement(int* tokenIndex, std::vector<Token> tokenList);
	TNode parseAssgnStatement(Token nameToken, int* tokenIndex, std::vector<Token> tokenList);
	TNode parseStatementList(int* tokenIndex, std::vector<Token> tokenList);
	TNode parseExpression(int* tokenIndex, std::vector<Token> tokenList);

	Token getNextToken(int* tokenIndex, std::vector<Token> tokenList);
	int getEndIndxOfStatementList(int* tokenIndex, std::vector<Token> tokenList);
};