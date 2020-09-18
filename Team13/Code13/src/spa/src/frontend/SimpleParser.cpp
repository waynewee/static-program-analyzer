#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <vector>

#include <ExprEvaluator.h>
#include <SimpleParser.h>
#include <TNode.h>
#include <Tokenizer.h>
#include <TokenType.h>
#include <Token.h>

using namespace std;

SimpleParser::SimpleParser() {}

TNode* SimpleParser::parse(string* input) {

	Tokenizer tokenizer(input);
	tokenizer.Tokenize();

	tokenList = tokenizer.GetTokenList();
	tokenIndx = -1;
	statementIndex = 0;

	TNode* mainPrg = new TNode(TNode::NODE_TYPE::program);
	
	while (tokenIndx < (int) tokenList.size() - 1) {
		Token procToken = SimpleParser::GetNextToken();

		if (procToken.GetValue() != TYPE_PROC) {
			throw logic_error("Missing procedure");
		}

		TNode* procNode = SimpleParser::ParseProcStatement();
		mainPrg->AddChild(procNode);
	}

	for (TNode* node : mainPrg->GetChildrenVector()) {
		node->Print(node);
	}
	return mainPrg;
}

// Returns a statement node
TNode* SimpleParser::ParseStatement() {
	Token firstToken = SimpleParser::GetNextToken();
	statementIndex++;

	switch (firstToken.GetTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (firstToken.GetStmtType()) {
		case TokenType::STMT_TYPE::_read:
			return SimpleParser::ParseReadStatement();
		case TokenType::STMT_TYPE::_print:
			return SimpleParser::ParsePrintStatement();
		case TokenType::STMT_TYPE::_call:
			return SimpleParser::ParseCallStatement();
		case TokenType::STMT_TYPE::_if:
			return SimpleParser::ParseIfStatement();
		case TokenType::STMT_TYPE::_while:
			return SimpleParser::ParseWhileStatement();
		default:
			throw logic_error("Unhandled statement name");
		}
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		return SimpleParser::ParseAssignStatement(firstToken);
	default:
		throw logic_error("Unhandled Token: " + firstToken.GetValue());
	}
}

TNode* SimpleParser::ParseProcStatement() {
	Token nameToken = SimpleParser::GetNextToken();
	if (nameToken.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw logic_error("Invalid procedure name");
	}
	
	TNode * procNode = new TNode(TNode::NODE_TYPE::procedure);
	TNode * procNameNode = new TNode(TNode::NODE_TYPE::procName, nameToken.GetValue());
	TNode * stmtListNode = SimpleParser::ParseStatementList();

	if (!procNode->AddChild(procNameNode) || !procNode->AddChild((stmtListNode))) {
		throw logic_error("Null node added to child of proc node");
	}
	
	return procNode;
}

TNode* SimpleParser::ParseReadStatement() {

	Token varToken = SimpleParser::GetNextToken();

	if (varToken.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw logic_error("Invalid variable name for read statement");
	}
	
	TNode * readNode = new TNode(TNode::NODE_TYPE::readStmt, statementIndex);
	TNode * varNode = new TNode(TNode::NODE_TYPE::varName, varToken.GetValue());

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw logic_error("Missing ';' in read statement");
	}

	if (!readNode->AddChild(varNode)) {
		throw logic_error("Null node added as child of read node");
	}

	return readNode;
}

TNode* SimpleParser::ParsePrintStatement() {
	Token varToken = SimpleParser::GetNextToken();
	if (varToken.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		
		throw logic_error("Invalid variable name for print statement");
	}

	TNode * printNode = new TNode (TNode::NODE_TYPE::printStmt, statementIndex);
	TNode * varNode = new TNode(TNode::NODE_TYPE::varName, varToken.GetValue());

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		// throw error for invalid statement
		throw logic_error("Missing ; in line: " + statementIndex);
	}

	if (!printNode->AddChild(varNode)) {
		throw logic_error("Null node added to child of print node");
	}

	return printNode;
}

TNode* SimpleParser::ParseCallStatement() {
	Token procNameToken = SimpleParser::GetNextToken();
	if (procNameToken.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw logic_error("Invalid procedure name for call statement");
	}
	
	TNode * callNode = new TNode(TNode::NODE_TYPE::callStmt, statementIndex);
	TNode * procNameNode = new TNode(TNode::NODE_TYPE::procName, statementIndex);

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON){
		throw logic_error("Missing ; in line: " + statementIndex);
	}

	if (!callNode->AddChild(procNameNode)) {
		throw logic_error("Null node added to child of call node");
	}
	return callNode;
}

TNode* SimpleParser::ParseIfStatement() {
	TNode* ifNode = new TNode(TNode::NODE_TYPE::ifStmt, statementIndex);
	TNode* exprNode = SimpleParser::ParseExpressionStatement(SimpleParser::expressionType::_if);
	TNode* thenStmtListNode = SimpleParser::ParseStatementList();

	for (TNode* child : thenStmtListNode->GetChildrenVector()) {
		child->SetParent(ifNode);
	}

	if (!ifNode->AddChild(exprNode) || !ifNode->AddChild(thenStmtListNode)) {
		throw logic_error("Null node added to child of if node");
	}

	if (PeekNextToken().GetValue() != TYPE_STMT_IF_ELSE) {
		throw logic_error("Missing Else Block");
	}

	GetNextToken(); // Iterating through 'else' keyword
	TNode * elseStmtListNode = SimpleParser::ParseStatementList();

	for (TNode* child : elseStmtListNode->GetChildrenVector()) {
		child->SetParent(ifNode);
	}

	if (!ifNode->AddChild(elseStmtListNode)) {
		throw logic_error("Null node added to child of if node");
	}

	return ifNode;
}

TNode* SimpleParser::ParseWhileStatement() {
	TNode* whleNode = new TNode(TNode::NODE_TYPE::whileStmt, statementIndex);
	TNode* exprNode = SimpleParser::ParseExpressionStatement(SimpleParser::expressionType::_while);
	TNode* stmtListNode = SimpleParser::ParseStatementList();

	for (TNode* child : stmtListNode->GetChildrenVector()) {
		child->SetParent(whleNode);
	}

	if (!whleNode->AddChild(exprNode) || !whleNode->AddChild(stmtListNode)) {
		throw logic_error("Null node added to child of while node");
	}

	return whleNode;
}


TNode* SimpleParser::ParseAssignStatement(Token nameToken) {
	TNode* assgnNode = new TNode(TNode::NODE_TYPE::assignStmt,  statementIndex);
	TNode* nameNode = new TNode(TNode::NODE_TYPE::varName, nameToken.GetValue());
	if (SimpleParser::GetNextToken().GetValue() != TYPE_EQUALS) {
		throw logic_error("Expected '=' token in line " + statementIndex);
	}

	TNode* exprNode = SimpleParser::ParseExpressionStatement(SimpleParser::expressionType::_assign);

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw logic_error("Missing ; in line: " + statementIndex);
	}
	if (!assgnNode->AddChild(nameNode) || !assgnNode->AddChild(exprNode)) {
		throw logic_error("Null node added to child of assign node");
	}
	return assgnNode;
}


// Returns a stmtList node
TNode* SimpleParser::ParseStatementList() {
	int endIndex = SimpleParser::GetEndIndxOfStatementList();
	TNode* stmtListNode = new TNode(TNode::NODE_TYPE::stmtList, statementIndex);
	// Parse the whole statement block 
	while ((tokenIndx) < endIndex) {
		if (PeekNextToken().GetValue() == TYPE_PUNC_OPEN_BRACKET ||
			PeekNextToken().GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
			GetNextToken();
			continue;
		}

		stmtListNode->AddChild(SimpleParser::ParseStatement());
	}

	return stmtListNode;
}

TNode* SimpleParser::ParseExpressionStatement(SimpleParser::expressionType exprType) {
	/**
	* An assign expression looks like
	* x + 1;
	*/
	vector<Token> exprList;

	Token nextToken;

	if (exprType == SimpleParser::expressionType::_assign) {

		while (SimpleParser::PeekNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
			nextToken = SimpleParser::GetNextToken();
			exprList.push_back(nextToken);
		}
	}
	else if (exprType == SimpleParser::expressionType::_if || exprType == SimpleParser::expressionType::_while) {

		if (SimpleParser::PeekNextToken().GetValue() != TYPE_PUNC_OPEN_PARAN) {
			throw logic_error("Invalid IF/WHILE statement");
		}

		if (exprType == SimpleParser::expressionType::_if) {
			
			while (SimpleParser::PeekNextToken().GetValue() != TYPE_STMT_IF_THEN) {
				nextToken = SimpleParser::GetNextToken();
				exprList.push_back(nextToken);
			}

			SimpleParser::GetNextToken();

		}
		else {
			while (SimpleParser::PeekNextToken().GetValue() != TYPE_PUNC_OPEN_BRACKET) {
				nextToken = SimpleParser::GetNextToken();
				exprList.push_back(nextToken);
			}
		}
	}
	TNode* exprNode = SimpleParser::ParseExpression(exprList);
	return exprNode;
	
}

TNode* SimpleParser::ParseExpression(vector<Token> exprList) {
	ExprEvaluator exprEvaluator(exprList);
	TNode* rootNode = exprEvaluator.Evaluate();
	return rootNode;
}

// Returns the number of tokens inside a statement list
int SimpleParser::GetEndIndxOfStatementList() {
	stack <Token> bracketMatcher;
	Token firstToken = SimpleParser::PeekNextToken();
	int counter = 1;

	if (firstToken.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
		bracketMatcher.push(firstToken);
		while (!bracketMatcher.empty()) {
			if (tokenIndx + 1 + counter > (int) tokenList.size()) {
				// throw Exception for incomplete statement block
				throw logic_error("Incomplete statement block");
			}
			Token nextToken = tokenList[tokenIndx + 1 + counter];
			if (nextToken.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
				bracketMatcher.push(nextToken);
			}
			if (nextToken.GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
				bracketMatcher.pop();
			}
			counter++;
		}
	}
	return tokenIndx + counter;
}

Token SimpleParser::GetNextToken() {
	tokenIndx++;
	return tokenList[tokenIndx];
}

Token SimpleParser::PeekNextToken() {
	return tokenList[tokenIndx + 1];
}
