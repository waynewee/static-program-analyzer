#include <vector>
#include <stack>
#include <queue>
#include <iostream>

#include "frontend/Tokenizer.h"
#include "Parser.h"
#include "frontend/TokenType.h"
#include "frontend/Token.h"
#include "frontend/Statement.h"
#include "frontend/ExprEvaluator.h"
#include "testUtils/TreeTraverse.h"

#include "pkb/TNode.h"

Parser::Parser() {
	std::cout << "Parser initialized" << std::endl;
	currentStatement = Statement();
}

std::vector<TNode*> Parser::parse(std::string input) {
	std::cout << "parse called" << std::endl;

	Tokenizer tokenizer(input);

	tokenizer.tokenize();

	tokenList = tokenizer.getTokenList();
	tokenIndx = -1;
	statementIndex = 0;

	while (tokenIndx < (int)tokenList.size() - 1) {
		Parser::parseStatement();
	}

	for (TNode* node : Parser::procNodesList) {
		std::cout << "Proc node: " << node->getData() << std::endl;
		for (TNode* child : node->getChildren()) {
			std::cout << child->getData() << std::endl;
		}
	}
	return Parser::procNodesList;
}

// Returns a statement node
TNode* Parser::parseStatement() {
	std::cout << "Parse Statement Called" << std::endl;
	Token firstToken = Parser::getNextToken();
	statementIndex++;

	switch (firstToken.getTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (firstToken.getStmtType()) {
		case TokenType::STMT_TYPE::_procedure: {
			TNode* procNode = Parser::parseProcStatement();
			Parser::procNodesList.push_back(procNode);
			return procNode;
		}
		case TokenType::STMT_TYPE::_read:
			return Parser::parseReadStatement();
		case TokenType::STMT_TYPE::_print:
			return Parser::parsePrintStatement();
		case TokenType::STMT_TYPE::_call:
			return Parser::parseCallStatement();
		case TokenType::STMT_TYPE::_if:
			return Parser::parseIfStatement();
		case TokenType::STMT_TYPE::_while:
			return Parser::parseWhileStatement();
		default:
			std::cout << "h2i" << std::endl;
			throw "Unhandled statement name";
		}
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		return Parser::parseAssgnStatement(firstToken);
	default:
		std::cout << "hi" << std::endl;
		throw "Unhandled Token: " + firstToken.getValue();
	}
}

TNode* Parser::parseProcStatement() {
	std::cout << "Parse ProcStatement Called" << std::endl;
	Token nameToken = Parser::getNextToken();
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid procedure name";
	}
	
	TNode * procNode = new TNode(TNode::NODE_TYPE::procedure);
	TNode * procNameNode = new TNode(TNode::NODE_TYPE::procName, nameToken.getValue());
	TNode * stmtListNode = Parser::parseStatementList();

	if (!procNode->AddChild((stmtListNode))) {
		throw "Null node added to child of proc node";
	}
	
	return procNode;
}

TNode* Parser::parseReadStatement() {
	std::cout << "Read called" << std::endl;

	Token varToken = Parser::getNextToken();

	if (varToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid variable name for read statement";
	}
	
	TNode * readNode = new TNode(TNode::NODE_TYPE::readStmt, statementIndex);
	TNode * varNode = new TNode(TNode::NODE_TYPE::varName, varToken.getValue());

	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ';' in read statement" ;
	}

	if (!readNode->AddChild(varNode)) {
		throw "Null node added as child of read node";
	}

	return readNode;
}

TNode* Parser::parsePrintStatement() {
	std::cout << "print called" << std::endl;

	Token varToken = Parser::getNextToken();
	if (varToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		
		throw "Invalid variable name for print statement";
	}

	TNode * printNode = new TNode (TNode::NODE_TYPE::printStmt, statementIndex);
	TNode * varNode = new TNode(TNode::NODE_TYPE::varName, varToken.getValue());

	if (Parser::getNextToken().getValue() != ";") {
		// throw error for invalid statement
		throw "Missing ; in line: " + statementIndex;
	}

	if (!printNode->AddChild(varNode)) {
		throw "Null node added to child of print node";
	}

	return printNode;
}

TNode* Parser::parseCallStatement() {
	std::cout << "Parse Call called" << std::endl;

	Token procNameToken = Parser::getNextToken();
	if (procNameToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid procedure name for call statement";
	}
	
	TNode * callNode = new TNode(TNode::NODE_TYPE::callStmt, statementIndex);
	TNode * procNameNode = new TNode(TNode::NODE_TYPE::procName, statementIndex);

	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ; in line: " + statementIndex;
	}

	if (!callNode->AddChild(procNameNode)) {
		throw "Null node added to child of call node";
	}
	return callNode;
}

TNode* Parser::parseIfStatement() {
	std::cout << "Parse If called" << std::endl;

	TNode* ifNode = new TNode(TNode::NODE_TYPE::ifStmt, statementIndex);
	TNode* exprNode = Parser::parseExpressionStatement(Parser::expressionType::IF);
	TNode* thenStmtListNode = Parser::parseStatementList();

	if (!ifNode->AddChild(exprNode) || !ifNode->AddChild(thenStmtListNode)) {
		throw "Null node added to child of if node";
	}

	if (peekNextToken().getValue() != "else") {
		throw "Missing Else Block";
	}

	getNextToken(); // Iterating through 'else' keyword
	TNode * elseStmtListNode = Parser::parseStatementList();

	if (!ifNode->AddChild(elseStmtListNode)) {
		throw "Null node added to child of if node";
	}
	return ifNode;
}

TNode* Parser::parseWhileStatement() {
	std::cout << "Parse While called" << std::endl;

	TNode* whleNode = new TNode(TNode::NODE_TYPE::whileStmt, statementIndex);
	TNode* exprNode = Parser::parseExpressionStatement(Parser::expressionType::WHILE);
	TNode* stmtListNode = Parser::parseStatementList();

	if (!whleNode->AddChild(exprNode) || !whleNode->AddChild(stmtListNode)) {
		throw "Null node added to child of while node";
	}

	return whleNode;
}


TNode* Parser::parseAssgnStatement(Token nameToken) {
	std::cout << "Parse Assign called" << std::endl;

	TNode* assgnNode = new TNode(TNode::NODE_TYPE::assignStmt,  statementIndex);

	if (Parser::getNextToken().getValue() != "=") {

		throw "Expected '=' token in line " + statementIndex;
	}

	TNode* exprNode = Parser::parseExpressionStatement(Parser::expressionType::ASSIGN);

	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ; in line: " + statementIndex;
	}
	if (!assgnNode->AddChild(exprNode)) {\
		throw "Null node added to child of assign node";
	}
	return assgnNode;
}


// Returns a stmtList node
TNode* Parser::parseStatementList() {
	std::cout << "Parse StatementList Called" << std::endl;
	int endIndex = Parser::getEndIndxOfStatementList();
	TNode* stmtListNode = new TNode(TNode::NODE_TYPE::stmtList, statementIndex);
	// Parse the whole statement block 
	while ((tokenIndx) < endIndex) {
		if (peekNextToken().getValue() == "{" ||
			peekNextToken().getValue() == "}") {
			getNextToken();
			continue;
		}

		stmtListNode->AddChild(Parser::parseStatement());
	}

	return stmtListNode;
}

TNode* Parser::parseExpressionStatement(Parser::expressionType exprType) {
	std::cout << "Parse ExpressionStatement called" << std::endl;

	/**
	* An assign expression looks like
	* x + 1;
	*/
	std::vector<Token> exprList;

	Token nextToken;

	if (exprType == Parser::expressionType::ASSIGN) {
		//std::cout << "Parsing ASSIGN expression" << std::endl;

		while (Parser::peekNextToken().getValue() != ";") {
			nextToken = Parser::getNextToken();
			exprList.push_back(nextToken);
		}
	}
	else if (exprType == Parser::expressionType::IF || exprType == Parser::expressionType::WHILE) {
		
		//std::cout << "Parsing IF/WHILE expression" << std::endl;

		if (Parser::peekNextToken().getValue() != "(") {
			throw "Invalid IF/WHILE statement";
		}

		if (exprType == Parser::expressionType::IF) {
			
			while (Parser::peekNextToken().getValue() != "then") {
				nextToken = Parser::getNextToken();
				exprList.push_back(nextToken);
			}

			Parser::getNextToken();

		}
		else {
			while (Parser::peekNextToken().getValue() != "{") {
				nextToken = Parser::getNextToken();
				exprList.push_back(nextToken);
			}
		}
	}
	TNode* exprNode = Parser::parseExpression(exprList);
	return exprNode;
	
}

int getPrecedence(Token t) {
		std::string val = t.getValue();

		if (val == "(" || val == ")") {
			return 7;
		}
		else if (val == "!") {
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

/**
* seems like everything is left asso
*/
bool isLeftAssoc(Token t) {

	std::string val = t.getValue();

	if (val == "(" || val == ")" || val == "<" || val == ">" || val == "<=" || val == ">=") {
		return false;
	}

	return true;
}

int compareOpPrecedence(Token a, Token b) {

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


TNode* Parser::parseExpression(std::vector<Token> exprList) {
	std::cout << "Parse Expression called" << std::endl;

	ExprEvaluator exprEvaluator(exprList);
	TNode* rootNode = exprEvaluator.evaluate();
	return rootNode;
}

// Returns the number of tokens inside a statement list
int Parser::getEndIndxOfStatementList() {
	std::stack <Token> bracketMatcher;
	Token firstToken = Parser::peekNextToken();
	int counter = 1;

	if (firstToken.getValue() == "{") {
		bracketMatcher.push(firstToken);
		while (!bracketMatcher.empty()) {
			if (tokenIndx + 1 + counter > (int) tokenList.size()) {
				// throw Exception for incomplete statement block
				std::cout << "Incomplete statement block" << std::endl;
			}
			Token nextToken = tokenList[tokenIndx + 1 + counter];
			if (nextToken.getValue() == "{") {
				bracketMatcher.push(nextToken);
			}
			if (nextToken.getValue() == "}") {
				bracketMatcher.pop();
			}
			counter++;
		}
	}
	return tokenIndx + counter;
}

Token Parser::getNextToken() {
	tokenIndx++;
	Parser::currentStatement.addToken(tokenList[tokenIndx]);
	return tokenList[tokenIndx];
}

Token Parser::peekNextToken() {
	return tokenList[tokenIndx + 1];
}
