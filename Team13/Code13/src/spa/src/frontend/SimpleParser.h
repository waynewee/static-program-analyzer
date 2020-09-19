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

	int statement_index_;
	int token_index_;
	TOKEN_LIST token_list_;

	TNode* Parse(TOKEN_LIST token_list);

private:
	TNode* ParseStatement(); 
	TNode* ParseProcStatement();
	TNode* ParseReadStatement();
	TNode* ParsePrintStatement();
	TNode* ParseIfStatement();
	TNode* ParseWhileStatement();
	TNode* ParseCallStatement();
	TNode* ParseAssignStatement(Token name_token);
	TNode* ParseStatementList();
	TNode* ParseExpressionStatement(expressionType expr_type);
	TNode* ParseExpression(TOKEN_LIST expr_list);

	Token GetNextToken();
	Token PeekNextToken();
	int GetEndIndxOfStatementList();
};

#endif