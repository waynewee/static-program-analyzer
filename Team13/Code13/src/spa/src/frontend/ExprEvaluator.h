#ifndef _EXPREVALUATOR_H_
#define _EXPREVALUATOR_H_

#include <queue>
#include <unordered_map>
#include <vector>

#include <pkb/TNode.h>
#include <Token.h>

using namespace std;

class ExprEvaluator {
public:

	vector<Token> tokenList;
	ExprEvaluator(vector<Token> exprList);
	TNode* evaluate();

	int unaryOpPrecedence = 6;

	unordered_map<string, int> precedenceMap = {
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
	queue<tuple<Token, TNode*>> shunt();
	TNode* evaluateQueue(queue<tuple<Token, TNode*>> shuntedQ);
	bool isLeftAssoc(Token t);
	int compareOpPrecedence(Token a, Token b);
	int getPrecedence(Token t);
	bool isUnaryOp(Token curr, Token prev);
	TNode::OPERATOR getOperator(string opStr);
	TNode* convertTokenToNode(Token t);
	
};

#endif