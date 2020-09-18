#ifndef _SIMPLEPARSER_H_
#define _SIMPLEPARSER_H_

#include <vector>

#include <FrontendTypes.h>
#include <TNode.h>
#include <Token.h>

using namespace std;

class SimpleParser {
public:
	enum expressionType { 
		_if, 
		_while, 
		_assign 
	};

	SimpleParser();

	int statementIndex;
	int tokenIndx;
	TOKEN_LIST tokenList;

	TNode* parse(string* input);

private:
	TNode* ParseStatement(); 
	TNode* ParseProcStatement();
	TNode* ParseReadStatement();
	TNode* ParsePrintStatement();
	TNode* ParseIfStatement();
	TNode* ParseWhileStatement();
	TNode* ParseCallStatement();
	TNode* ParseAssignStatement(Token nameToken);
	TNode* ParseStatementList();
	TNode* ParseExpressionStatement(expressionType exprType);
	TNode* ParseExpression(TOKEN_LIST exprList);

	Token GetNextToken();
	Token PeekNextToken();
	int GetEndIndxOfStatementList();
};

#endif