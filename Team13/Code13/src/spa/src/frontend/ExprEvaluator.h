#pragma once
#include <vector>
#include <queue>

#include "Token.h"
#include "pkb/TNode.h"
#include "Statement.h"

class ExprEvaluator {
public:

	std::vector<Token> tokenList;
	ExprEvaluator(std::vector<Token> exprList);
	TNode evaluate();

private:
	std::queue<Token> shunt();
	TNode evaluateQueue(std::queue<Token> shuntedQ);
	bool isLeftAssoc(Token t);
	int compareOpPrecedence(Token a, Token b);
	int getPrecedence(Token t);
	bool isUnaryOp(Token curr, Token prev);
	
};