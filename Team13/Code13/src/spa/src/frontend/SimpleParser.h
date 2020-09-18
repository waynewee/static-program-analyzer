#ifndef _SIMPLEPARSER_H_
#define _SIMPLEPARSER_H_

#include <vector>

#include <TNode.h>
#include <Token.h>

using namespace std;

class SimpleParser {
public:
	SimpleParser();
	int tokenIndx;
	int statementIndex;
	vector<Token> tokenList;

	TNode* parse(string input);
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
	TNode* parseExpression(vector<Token> exprList);

	Token getNextToken();
	Token peekNextToken();
	int getEndIndxOfStatementList();
};

#endif