#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <vector>

#include "Token.h"
#include "pkb/TNode.h"

class ExprEvaluator {
public:

	std::vector<Token> tokenList;
	ExprEvaluator(std::vector<Token> exprList);
	TNode* evaluate();

	int unaryOpPrecedence = 6;

	std::unordered_map<std::string, int> precedenceMap = {
	{"(", 7},
	{")", 7},
	{"!", 6},
	{"*", 5},
	{"%", 5},
	{"+", 4},
	{"-", 4},
	{">", 3},
	{"<", 3},
	{"<=", 3},
	{">=", 3},
	{"==", 2},
	{"!=", 2},
	{"&&", 1},
	{"||", 0},
	};

private:
	std::queue<tuple<Token, TNode*>> shunt();
	TNode* evaluateQueue(std::queue<tuple<Token, TNode*>> shuntedQ);
	bool isLeftAssoc(Token t);
	int compareOpPrecedence(Token a, Token b);
	int getPrecedence(Token t);
	bool isUnaryOp(Token curr, Token prev);
	TNode::OPERATOR getOperator(std::string opStr);
	TNode* convertTokenToNode(Token t);
	
};