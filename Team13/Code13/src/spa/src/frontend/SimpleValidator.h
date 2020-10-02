#ifndef _SIMPLEVALIDATOR_H_
#define _SIMPLEVALIDATOR_H_

#include <vector>

#include "FrontendTypes.h"

using namespace std;

class SimpleValidator {
public:
	SimpleValidator();

	int token_index_;
	int statement_index_;
	TOKEN_LIST token_list_;

	bool IsValid(TOKEN_LIST token_list);

private:
	bool IsValidStmt();
	bool IsValidProc();
	bool IsValidReadStmt();
	bool IsValidPrintStmt();
	bool IsValidIfBlock();
	bool IsValidWhileBlock();
	bool IsValidCallStmt();
	bool IsValidAssignment(Token name_token);

	//hmmmm??
	bool IsValidStmtList();
	bool IsValidExpressionStmt(expressionType expr_type);
	bool IsValidExpression(TOKEN_LIST expr_list);

	Token GetNextToken();
	Token PeekNextToken();
	int GetEndIndxOfStatementList();
};

#endif