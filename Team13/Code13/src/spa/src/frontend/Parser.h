#pragma once
#include <vector>

#include "Token.h"
#include "TNode.h"

class Parser {
public:
	Parser();
	int tokenIndx;
	int statementIndex;
	std::vector<Token> tokenList;

	TNode* parse(std::string input);
	enum expressionType { IF, WHILE, ASSIGN };

private:
	TNode* parseStatement(); 
	TNode* parseProcStatement();
	TNode* parseReadStatement();
	TNode* parsePrintStatement();
	TNode* parseIfStatement();
	TNode* parseWhileStatement();
	TNode* parseCallStatement();
	TNode* parseAssgnStatement(Token nameToken);
	TNode* parseStatementList();
	TNode* parseExpressionStatement(expressionType exprType);
	TNode* parseExpression(std::vector<Token> exprList);

	Token getNextToken();
	Token peekNextToken();
	int getEndIndxOfStatementList();
};