#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#include <CustomTypes.h>
#include <ExprEvaluator.h>
#include <FrontendTypes.h>
#include <pkb/TNode.h>
#include <testUtils/TreeTraverse.h>
#include <Token.h>
#include <TokenType.h>

using namespace std;

ExprEvaluator::ExprEvaluator(TOKEN_LIST exprList) {
	tokenList = exprList;
}

TNode* ExprEvaluator::Evaluate() {
	return EvaluateQueue(Shunt());
}

TNode* ExprEvaluator::EvaluateQueue( queue<tuple<Token, TNode*>> shuntedQ ) {

	stack<tuple<Token, TNode*>> tStack;

	tuple<Token, TNode*> tup = shuntedQ.front();
	Token token = get<0>(tup);
	TNode* rootNodePtr = get<1>(tup);

	while (shuntedQ.size() > 0) {

		tup = shuntedQ.front();

		token = get<0>(tup);
		shuntedQ.pop();

		if (token.GetTokenType() == TokenType::TOKEN_TYPE::constant || token.GetTokenType() == TokenType::TOKEN_TYPE::var) {
			tStack.push(tup);
		}
		//guaranteed to be an expr
		else {

			//CREATE PARENT NODE
			TNode* parentNodePtr = get<1>(tup);
			rootNodePtr = parentNodePtr;

			/**
			* Check size of stack, if size == 1, then is unary
			*/
			if (token.isUnaryOp) {

				//CREATE SINGLE CHILD NODE
				TNode* cNodePtr = get<1>(tStack.top());
				tStack.pop();
				
				//LINK PARENT TO CHILD
				parentNodePtr->AddChild(cNodePtr);

			}
			else {

				//CREATE RIGHT CHILD NODE
				TNode* rNodePtr = get<1>(tStack.top());
				tStack.pop();
				
				//CREATE LEFT CHILD NODE
				TNode* lNodePtr = get<1>(tStack.top());
				tStack.pop();

				parentNodePtr->AddChild(lNodePtr);
				parentNodePtr->AddChild(rNodePtr);
			}

			tStack.push(tup);

		}

	}

	if (rootNodePtr != NULL) {
		return rootNodePtr;
	}
	return NULL;

}

queue<tuple<Token, TNode*>> ExprEvaluator::Shunt() {
	queue<tuple<Token, TNode*>> outputQ;
	stack<Token> opStack;

	int i = 0;

	while( i < tokenList.size()){

		Token token = tokenList.at(i);
		
		TokenType::TOKEN_TYPE tokenType = token.GetTokenType();

		if (tokenType == TokenType::TOKEN_TYPE::constant || tokenType == TokenType::TOKEN_TYPE::var) {
			outputQ.push(make_tuple(token, ConvertTokenToNode(token)));
		}
		else if (tokenType == TokenType::TOKEN_TYPE::expr|| tokenType == TokenType::TOKEN_TYPE::rel_expr) {

			while (opStack.size() > 0
				&& (
					CompareOpPrecedence(token, opStack.top()) == -1
					|| (
						CompareOpPrecedence(token, opStack.top()) == 0
						&& IsLeftAssoc(token)
						)
					)
				&& opStack.top().GetValue() != TYPE_PUNC_OPEN_PARAN
				) {
				Token opToken = opStack.top();
				opStack.pop();
				outputQ.push(make_tuple(opToken, ConvertTokenToNode(opToken)));
			}

			opStack.push(token);
		}
		else if ( (token.GetValue() == TYPE_PUNC_OPEN_PARAN) || i >= 1 && token.isUnaryOp)
		{
			opStack.push(token);
		}
		else if (token.GetValue() == TYPE_PUNC_CLOSED_PARAN) {

			while (opStack.top().GetValue() != TYPE_PUNC_OPEN_PARAN) {
				if (opStack.size() == 0) {
					throw logic_error("Mismatched parantheses!");
				}

				Token opToken = opStack.top();
				opStack.pop();
				outputQ.push(make_tuple(opToken, ConvertTokenToNode(opToken)));

			}

			if (opStack.top().GetValue() == TYPE_PUNC_OPEN_PARAN) {
				opStack.pop();
			}
		}

		i++;
	}

	while (opStack.size() > 0) {
		Token opToken = opStack.top();
		opStack.pop();
		outputQ.push(make_tuple(opToken, ConvertTokenToNode(opToken)));
	}

	return outputQ;
}

bool ExprEvaluator::IsLeftAssoc(Token t) {

	string val = t.GetValue();

	for (string s : rightAssocList) {
		if (s == val) {
			return false;
		}
	}

}

int ExprEvaluator::CompareOpPrecedence(Token a, Token b) {

	int aPrecedence = GetPrecedence(a);
	int bPrecedence = GetPrecedence(b);

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

int ExprEvaluator::GetPrecedence(Token t) {
	string val = t.GetValue();
	
	if (t.isUnaryOp) {
		return PRECEDENCE_UNARY;
	}

	return precedenceMap[val];
}

TNode::OPERATOR ExprEvaluator::GetOperator(string opStr) {
	if (opStr == TYPE_REL_EXPR_NOT) {
		return TNode::OPERATOR::notOp;
	}
	else if (opStr == TYPE_REL_EXPR_AND) {
		return TNode::OPERATOR::andOp;
	}
	else if (opStr == TYPE_REL_EXPR_OR) {
		return TNode::OPERATOR::orOp;
	}
	else if (opStr == TYPE_REL_EXPR_GT) {
		return TNode::OPERATOR::greater;
	}
	else if (opStr == TYPE_REL_EXPR_GTE) {
		return TNode::OPERATOR::geq;
	}
	else if (opStr == TYPE_REL_EXPR_LT) {
		return TNode::OPERATOR::lesser;
	}
	else if (opStr == TYPE_REL_EXPR_LTE) {
		return TNode::OPERATOR::notOp;
	}
	else if (opStr == TYPE_REL_EXPR_EQ) {
		return TNode::OPERATOR::equal;
	}
	else if (opStr == TYPE_REL_EXPR_NEQ) {
		return TNode::OPERATOR::unequal;
	}
	else if (opStr == TYPE_EXPR_PLUS) {
		return TNode::OPERATOR::plus;
	}
	else if (opStr == TYPE_EXPR_MINUS) {
		return TNode::OPERATOR::minus;
	}
	else if (opStr == TYPE_EXPR_TIMES) {
		return TNode::OPERATOR::times;
	}
	else if (opStr == TYPE_EXPR_DIVIDE) {
		return TNode::OPERATOR::divide;
	}
	else if (opStr == TYPE_EXPR_MOD) {
		return TNode::OPERATOR::mod;
	}
	else {
		throw logic_error("Invalid token");
	}

}

TNode* ExprEvaluator::ConvertTokenToNode(Token t) {

	//token can only be expr (+, -, * etc), rel_expr (&&, || etc), variable, constant

	if (t.GetValue() == TYPE_PUNC_OPEN_PARAN || t.GetValue() == TYPE_PUNC_CLOSED_PARAN) {
		return new TNode();
	}
	if (t.GetTokenType() == TokenType::TOKEN_TYPE::expr) {
		return new TNode(TNode::NODE_TYPE::expr, GetOperator(t.GetValue()));
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::rel_expr) {
		return new TNode(TNode::NODE_TYPE::relExpr, GetOperator(t.GetValue()));
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::var) {
		return new TNode(TNode::NODE_TYPE::varName, t.GetValue());
	}
	else if (t.GetTokenType() == TokenType::TOKEN_TYPE::constant) {
		return new TNode(TNode::NODE_TYPE::constValue, stod(t.GetValue()));
	}
	else {
		throw logic_error("Invalid token");
	}

};
