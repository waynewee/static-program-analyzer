#pragma once
#include <vector>

#include "frontend/Token.h"
#include "pkb/TNode.h"
#include "frontend/Statement.h"

class Parser {
public:
	Parser();
	int tokenIndx;
	int statementIndex;
	std::vector<Token> tokenList;
	std::vector<Statement> statementList;
	std::vector<TNode> procNodesList;
	Statement currentStatement;

	std:: vector<TNode> parse(std::string input);
	enum expressionType { IF, WHILE, ASSIGN };

private:
	TNode parseStatement(); 
	TNode parseProcStatement();
	TNode parseReadStatement();
	TNode parsePrntStatement();
	TNode parseIfStatement();
	TNode parseWhleStatement();
	TNode parseCallStatement();
	TNode parseAssgnStatement(Token nameToken);
	TNode parseStatementList();
	TNode parseExpressionStatement(expressionType exprType);
	TNode parseExpression(std::vector<Token> exprList);

	Token getNextToken();
	Token peekNextToken();
	int getEndIndxOfStatementList();
};