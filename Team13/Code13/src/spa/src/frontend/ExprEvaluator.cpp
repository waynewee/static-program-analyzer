#include "ExprEvaluator.h"
#include "Token.h"
#include "TokenType.h"
#include "pkb/TNode.h"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

ExprEvaluator::ExprEvaluator(std::vector<Token> exprList) {
	tokenList = exprList;

	/*for (Token t : exprList) {
		t.print();
	}*/
}

TNode ExprEvaluator::evaluate() {

	return evaluateQueue(shunt());
}

TNode ExprEvaluator::evaluateQueue( std::queue<tuple<Token, TNode>> shuntedQ ) {

	std::stack<tuple<Token, TNode>*> tStack;

	TNode rootNode = std::get<1>(shuntedQ.front());

	while (shuntedQ.size() > 0) {

		tuple<Token, TNode>* tupPtr = &shuntedQ.front();
		Token* tokenPtr = &std::get<0>(shuntedQ.front());
		std::cout << tokenPtr->getValue() << std::endl;
		TNode* nodePtr = &std::get<1>(shuntedQ.front());

		shuntedQ.pop();
				
		if (tokenPtr->getTokenType() == TokenType::TOKEN_TYPE::constant || tokenPtr->getTokenType() == TokenType::TOKEN_TYPE::var) {
			tStack.push(tupPtr);
		}
		//guaranteed to be an expr
		else {

			/**
			* Check size of stack, if size == 1, then is unary
			*/
			if (tokenPtr->isUnaryOp) {
				/**
				* Pop off one kid
				*/
				tuple<Token, TNode>* cTup= tStack.top();
				TNode* cNodePtr = &std::get<1>(*cTup);
				nodePtr->AddChild(cNodePtr);
				tStack.pop();

			}
			else {
				/**
				* Pop off two kids
				*/
				tuple<Token, TNode>* rTup = tStack.top();
				TNode* rNodePtr = &std::get<1>(*rTup);
				tStack.pop();
				tuple<Token, TNode>* lTup= tStack.top();
				TNode* lNodePtr = &std::get<1>(*lTup);
				tStack.pop();
				
				std::printf("Parent Address: %p\n", nodePtr);
				std::printf("Left Child Address: %p\n", lNodePtr);
				std::printf("Right Child Address: %p\n", rNodePtr);

				nodePtr->AddChild(rNodePtr);
				nodePtr->AddChild(lNodePtr);

			}

			tStack.push(tupPtr);

		}

		std::cout << shuntedQ.size() << std::endl;

	}

	return rootNode;

}

std::queue<tuple<Token, TNode>> ExprEvaluator::shunt() {
	std::queue<tuple<Token, TNode>> outputQ;
	std::stack<Token> opStack;

	int i = 0;

	while( i < tokenList.size()){

		Token token = tokenList.at(i);
		TNode node = convertTokenToNode(token);

		TokenType::TOKEN_TYPE tokenType = token.getTokenType();

		if (tokenType == TokenType::TOKEN_TYPE::constant || tokenType == TokenType::TOKEN_TYPE::var) {
			outputQ.push(make_tuple(token, node));
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
				TNode opNode = convertTokenToNode(opToken);
				opStack.pop();
				outputQ.push(make_tuple(opToken, opNode));
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
				TNode opNode = convertTokenToNode(opToken);
				opStack.pop();
				outputQ.push(make_tuple(opToken, opNode));

			}

			if (opStack.top().getValue() == "(") {
				opStack.pop();
			}
		}

		i++;
	}

	while (opStack.size() > 0) {
		Token opToken = opStack.top();
		TNode opNode = convertTokenToNode(opToken);
		opStack.pop();
		outputQ.push(make_tuple(opToken, opNode));
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

TNode ExprEvaluator::convertTokenToNode(Token t) {

	//token can only be expr (+, -, * etc), rel_expr (&&, || etc), variable, constant

	if (t.getValue() == "(" || t.getValue() == ")") {
		return TNode();
	}
	if (t.getTokenType() == TokenType::TOKEN_TYPE::expr) {
		return TNode(TNode::NODE_TYPE::expr, getOperator(t.getValue()));
	}
	else if (t.getTokenType() == TokenType::TOKEN_TYPE::rel_expr) {
		return TNode(TNode::NODE_TYPE::relExpr, getOperator(t.getValue()));
	}
	else if (t.getTokenType() == TokenType::TOKEN_TYPE::var) {
		return TNode(TNode::NODE_TYPE::varName, t.getValue());
	}
	else if (t.getTokenType() == TokenType::TOKEN_TYPE::constant) {
		return TNode(TNode::NODE_TYPE::constValue, std::stod(t.getValue()));
	}
	else {
		throw "Invalid token";
	}

};
