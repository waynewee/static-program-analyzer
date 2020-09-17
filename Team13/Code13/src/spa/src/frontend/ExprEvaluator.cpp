#include "ExprEvaluator.h"
#include "Token.h"
#include "TokenType.h"
#include "pkb/TNode.h"
#include "testUtils/TreeTraverse.h"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

ExprEvaluator::ExprEvaluator(std::vector<Token> exprList) {
	tokenList = exprList;
}

TNode* ExprEvaluator::evaluate() {

	return evaluateQueue(shunt());
}

TNode* ExprEvaluator::evaluateQueue( std::queue<tuple<Token, TNode*>> shuntedQ ) {

	std::stack<tuple<Token, TNode*>> tStack;

	TNode* rootNodePtr = NULL;

	while (shuntedQ.size() > 0) {

		tuple<Token, TNode*> tup = shuntedQ.front();

		Token token = std::get<0>(tup);
		shuntedQ.pop();

		if (token.getTokenType() == TokenType::TOKEN_TYPE::constant || token.getTokenType() == TokenType::TOKEN_TYPE::var) {
			tStack.push(tup);
		}
		//guaranteed to be an expr
		else {

			//CREATE PARENT NODE
			TNode* parentNodePtr = std::get<1>(tup);
			rootNodePtr = parentNodePtr;

			/**
			* Check size of stack, if size == 1, then is unary
			*/
			if (token.isUnaryOp) {

				//CREATE SINGLE CHILD NODE
				TNode* cNodePtr = std::get<1>(tStack.top());
				tStack.pop();
				
				//LINK PARENT TO CHILD
				parentNodePtr->AddChild(cNodePtr);

			}
			else {

				//CREATE RIGHT CHILD NODE
				TNode* rNodePtr = std::get<1>(tStack.top());
				tStack.pop();
				
				//CREATE LEFT CHILD NODE
				TNode* lNodePtr = std::get<1>(tStack.top());
				tStack.pop();

				parentNodePtr->AddChild(lNodePtr);
				parentNodePtr->AddChild(rNodePtr);
			}

			tStack.push(tup);

		}

	}

	if (rootNodePtr != NULL) {

		TreeTraverse::traverse(rootNodePtr);

		return rootNodePtr;
	}

	return NULL;

}

std::queue<tuple<Token, TNode*>> ExprEvaluator::shunt() {
	std::queue<tuple<Token, TNode*>> outputQ;
	std::stack<Token> opStack;

	int i = 0;

	while( i < tokenList.size()){

		Token token = tokenList.at(i);
		
		TokenType::TOKEN_TYPE tokenType = token.getTokenType();

		if (tokenType == TokenType::TOKEN_TYPE::constant || tokenType == TokenType::TOKEN_TYPE::var) {
			outputQ.push(make_tuple(token, convertTokenToNode(token)));
		}
		else if (tokenType == TokenType::TOKEN_TYPE::expr|| tokenType == TokenType::TOKEN_TYPE::rel_expr) {

			while (opStack.size() > 0
				&& (
					compareOpPrecedence(token, opStack.top()) == -1
					|| (
						compareOpPrecedence(token, opStack.top()) == 0
						&& isLeftAssoc(token)
						)
					)
				&& opStack.top().getValue() != "("
				) {
				Token opToken = opStack.top();
				opStack.pop();
				outputQ.push(make_tuple(opToken, convertTokenToNode(opToken)));
			}

			opStack.push(token);
		}
		else if ( (token.getValue() == "(") || i >= 1 && token.isUnaryOp)
		{
			opStack.push(token);
		}
		else if (token.getValue() == ")") {

			while (opStack.top().getValue() != "(") {
				if (opStack.size() == 0) {
					throw "mismatched parantheses!";
				}

				Token opToken = opStack.top();
				opStack.pop();
				outputQ.push(make_tuple(opToken, convertTokenToNode(opToken)));

			}

			if (opStack.top().getValue() == "(") {
				opStack.pop();
			}
		}

		i++;
	}

	while (opStack.size() > 0) {
		Token opToken = opStack.top();
		opStack.pop();
		outputQ.push(make_tuple(opToken, convertTokenToNode(opToken)));
	}

	return outputQ;
}

bool ExprEvaluator::isLeftAssoc(Token t) {

	std::string val = t.getValue();

	if (val == "(" || val == ")" || val == "<" || val == ">" || val == "<=" || val == ">=") {
		return false;
	}

	return true;
}

int ExprEvaluator::compareOpPrecedence(Token a, Token b) {

	int aPrecedence = getPrecedence(a);
	int bPrecedence = getPrecedence(b);

	if (aPrecedence == bPrecedence) {
		return 0;
	}
	else if (aPrecedence < bPrecedence) {
		return -1;
	}
	else {
		return 1;
	}
}

int ExprEvaluator::getPrecedence(Token t) {
	std::string val = t.getValue();
	
	if (t.isUnaryOp) {
		return unaryOpPrecedence;
	}

	return precedenceMap[val];
}

TNode::OPERATOR ExprEvaluator::getOperator(std::string opStr) {
	if (opStr == "!") {
		return TNode::OPERATOR::notOp;
	}
	else if (opStr == "&&") {
		return TNode::OPERATOR::andOp;
	}
	else if (opStr == "||") {
		return TNode::OPERATOR::orOp;
	}
	else if (opStr == ">") {
		return TNode::OPERATOR::greater;
	}
	else if (opStr == ">=") {
		return TNode::OPERATOR::geq;
	}
	else if (opStr == "<") {
		return TNode::OPERATOR::lesser;
	}
	else if (opStr == "<=") {
		return TNode::OPERATOR::notOp;
	}
	else if (opStr == "==") {
		return TNode::OPERATOR::equal;
	}
	else if (opStr == "!=") {
		return TNode::OPERATOR::unequal;
	}
	else if (opStr == "+") {
		return TNode::OPERATOR::plus;
	}
	else if (opStr == "-") {
		return TNode::OPERATOR::minus;
	}
	else if (opStr == "*") {
		return TNode::OPERATOR::times;
	}
	else if (opStr == "/") {
		return TNode::OPERATOR::divide;
	}
	else if (opStr == "%") {
		return TNode::OPERATOR::mod;
	}
	else {
		throw "Invlaid token";
	}

}

TNode* ExprEvaluator::convertTokenToNode(Token t) {

	//token can only be expr (+, -, * etc), rel_expr (&&, || etc), variable, constant

	if (t.getValue() == "(" || t.getValue() == ")") {
		return new TNode();
	}
	if (t.getTokenType() == TokenType::TOKEN_TYPE::expr) {
		return new TNode(TNode::NODE_TYPE::expr, getOperator(t.getValue()));
	}
	else if (t.getTokenType() == TokenType::TOKEN_TYPE::rel_expr) {
		return new TNode(TNode::NODE_TYPE::relExpr, getOperator(t.getValue()));
	}
	else if (t.getTokenType() == TokenType::TOKEN_TYPE::var) {
		return new TNode(TNode::NODE_TYPE::varName, t.getValue());
	}
	else if (t.getTokenType() == TokenType::TOKEN_TYPE::constant) {
		return new TNode(TNode::NODE_TYPE::constValue, std::stod(t.getValue()));
	}
	else {
		throw "Invalid token";
	}

};
