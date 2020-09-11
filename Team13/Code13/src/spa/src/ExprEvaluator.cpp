#include "ExprEvaluator.h"
#include "Token.h"
#include "TokenType.h"
#include "TNode.h"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

ExprEvaluator::ExprEvaluator(std::vector<Token> exprList) {
	tokenList = exprList;

	for (Token t : exprList) {
		t.print();
	}
}

TNode ExprEvaluator::evaluate() {
	return evaluateQueue(shunt());
}

TNode ExprEvaluator::evaluateQueue( std::queue<Token> shuntedQ ) {

	std::stack<Token> tStack;

	while (shuntedQ.size() > 0) {
		Token token = shuntedQ.front();
		shuntedQ.pop();
				
		if (token.getTokenType() == TokenType::TOKEN_TYPE::INTR || token.getTokenType() == TokenType::TOKEN_TYPE::NAME) {
			tStack.push(token);
		}
		//guaranteed to be an operator
		else {
			//TNode node = createNode(token)

			/**
			* Check size of stack, if size == 1, then is unary
			*/
			if (token.isUnaryOp) {
				/**
				* Pop off one kid
				*/
				Token cToken = tStack.top();
				tStack.pop();

				std::cout << cToken.getValue() + "___" + token.getValue() << std::endl;
			}
			else {
				/**
				* Pop off two kids
				*/
				Token rToken = tStack.top();
				tStack.pop();

				Token lToken = tStack.top();
				tStack.pop();

				std::cout << lToken.getValue() + "___" + token.getValue() + "___" + rToken.getValue() << std::endl;

			}

			tStack.push(token);


		}

	}

	TNode emptyNode;
	return emptyNode;

}

std::queue<Token> ExprEvaluator::shunt() {
	std::queue<Token> outputQ;
	std::stack<Token> opStack;

	int i = 0;

	while( i < tokenList.size()){

		Token token = tokenList.at(i);

		TokenType::TOKEN_TYPE tokenType = token.getTokenType();

		if (tokenType == TokenType::TOKEN_TYPE::INTR || tokenType == TokenType::TOKEN_TYPE::NAME) {
			outputQ.push(token);
		}
		else if (tokenType == TokenType::TOKEN_TYPE::AROP || tokenType == TokenType::TOKEN_TYPE::LGOP) {

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
				outputQ.push(opToken);
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
				outputQ.push(opToken);

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
		outputQ.push(opToken);
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

	if (val == "(" || val == ")") {
		return 7;
	}
	else if (val == "!" || val == "-" && t.isUnaryOp) {
		return 6;
	}
	else if (val == "*" || val == "%") {
		return 5;
	}
	else if (val == "+" || val == "-") {
		return 4;
	}
	else if (val == "<" || val == ">" || val == "<=" || val == ">=") {
		return 3;
	}
	else if (val == "==" || val == "!=") {
		return 2;
	}
	else if (val == "&&") {
		return 1;
	}
	else if (val == "||") {
		return 0;
	}

	std::cout << "Invalid token" << std::endl;

	throw "Invalid token";
}