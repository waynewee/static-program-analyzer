#pragma once
#include <vector>
#include <queue>

#include "Token.h"
#include "TNode.h"
#include "Statement.h"

class ExprEvaluator {
public:

	ExprEvaluator(std::vector<Token> exprList);
	TNode evaluate();

private:
	std::vector<Token> tokenList;
	std::queue<Token> shunt();
	TNode evaluateQueue(std::queue<Token> shuntedQ);
	bool isLeftAssoc(Token t);
	int compareOpPrecedence(Token a, Token b);
	int getPrecedence(Token t);
	
};