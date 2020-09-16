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
}

TNode ExprEvaluator::evaluate() {

	return evaluateQueue(shunt());
}

TNode ExprEvaluator::evaluateQueue( std::queue<Token> shuntedQ ) {

	std::stack<Token> tStack;

	while (shuntedQ.size() > 0) {

		//CREATE PARENT NODE
		Token token = shuntedQ.front();
		shuntedQ.pop();
				
		if (token.getTokenType() == TokenType::TOKEN_TYPE::constant || token.getTokenType() == TokenType::TOKEN_TYPE::var) {
			tStack.push(token);
		}
		//guaranteed to be an expr
		else {

			/**
			* Check size of stack, if size == 1, then is unary
			*/
			if (token.isUnaryOp) {

				//CREATE SINGLE CHILD NODE
				Token cToken = tStack.top();
				tStack.pop();

				//LINK PARENT TO CHILD
				std::cout << cToken.getValue() + "_____" +  token.getValue() << std::endl;


			}
			else {

				//CREATE RIGHT CHILD NODE
				Token rToken = tStack.top();
				tStack.pop();
				
				//CREATE LEFT CHILD NODE
				Token lToken = tStack.top();
				tStack.pop();

				//LINK PARENT TO CHILDREN
				std::cout << lToken.getValue() + "______" + token.getValue() + "______" + rToken.getValue() << std::endl;
			}

			tStack.push(token);

		}

	}

	//RETURN THE LAST PARENT NODE!
	TNode rootNode;
	return rootNode;

}

std::queue<Token> ExprEvaluator::shunt() {
	std::queue<Token> outputQ;
	std::stack<Token> opStack;

	int i = 0;

	while( i < tokenList.size()){

		Token token = tokenList.at(i);
		
		TokenType::TOKEN_TYPE tokenType = token.getTokenType();

		if (tokenType == TokenType::TOKEN_TYPE::constant || tokenType == TokenType::TOKEN_TYPE::var) {
			outputQ.push(token);
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
