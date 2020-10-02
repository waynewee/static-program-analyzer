/*#ifndef _ASTBUILDER_H_
#define _ASTBUILDER_H_

#include <vector>

#include "FrontendTypes.h"
#include <TNode.h>
#include "Token.h"

using namespace std;

class ASTBuilder {
public:
	enum expressionType {
		_if,
		_while,
		_assign
	};

	ASTBuilder();

	int statement_index_;
	int token_index_;
	TOKEN_LIST token_list_;

	TNode* BuildMainPrgNode(TOKEN_LIST token_list);

private:
	TNode* BuildStatement();
	TNode* BuildProcStatement();
	TNode* BuildReadStatement();
	TNode* BuildPrintStatement();
	TNode* BuildIfStatement();
	TNode* BuildWhileStatement();
	TNode* BuildCallStatement();
	TNode* BuildAssignStatement(Token name_token);
	TNode* BuildStatementList();
	TNode* BuildExpressionStatement(expressionType expr_type);
	TNode* BuildExpression(TOKEN_LIST expr_list);

	Token GetNextToken();
	Token PeekNextToken();
	int GetEndIndxOfStatementList();
};

#endif*/