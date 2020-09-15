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

#include "pkb/TNode.h"

Parser::Parser() {
	currentStatement = Statement();
}

std::vector<TNode> Parser::parse(std::string input) {

	Tokenizer tokenizer(input);

	tokenizer.tokenize();

	tokenList = tokenizer.getTokenList();
	tokenIndx = -1;
	statementIndex = 0;

	while (tokenIndx < (int)tokenList.size() - 1) {
		Parser::parseStatement();
	}

	for (TNode n : Parser::procNodesList) {
		n.Print(&n);
	}

	return Parser::procNodesList;
}

// Returns a statement node
TNode Parser::parseStatement() {
	//std::cout << "Parse Statement Called" << std::endl;
	Token firstToken = Parser::getNextToken();
	statementIndex++;
	TNode statementNode = TNode();

	switch (firstToken.getTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (firstToken.getStmtType()) {
		case TokenType::STMT_TYPE::_procedure:
			statementNode = Parser::parseProcStatement();
			Parser::currentStatement.print();
			Parser::currentStatement = Statement();
			Parser::procNodesList.push_back(statementNode);
			break;
		case TokenType::STMT_TYPE::_read:
			statementNode = Parser::parseReadStatement();
			break;
		case TokenType::STMT_TYPE::_print:
			statementNode = Parser::parsePrintStatement();
			break;
		case TokenType::STMT_TYPE::_call:
			statementNode = Parser::parseCallStatement();
			break;
		case TokenType::STMT_TYPE::_if:
			statementNode = Parser::parseIfStatement();
			break;
		case TokenType::STMT_TYPE::_while:
			statementNode = Parser::parseWhileStatement();
			break;
		default:
			throw "Unhandled statement name";
			break;
		}
		break;
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		statementNode = Parser::parseAssgnStatement(firstToken);
		break;
	default:
		throw "Unhandled Token: " + firstToken.getValue();
	}

	return statementNode;
}


//	TNode(NODE_TYPE _type, VAR_NAME _name, CONST_VALUE _value, OPERATOR _op, STMT_IDX _index);

TNode Parser::parseProcStatement() {
	//std::cout << "Parse ProcStatement Called" << std::endl;
	Token nameToken = Parser::getNextToken();
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid procedure name";
	}
	
	TNode procNode(TNode::NODE_TYPE::procedure);
	TNode procNameNode(TNode::NODE_TYPE::procName, nameToken.getValue());

	TNode* procNameNodePtr = &procNameNode;
	
	bool addedProcName = procNode.AddChild(procNameNodePtr);

	int endIndex = Parser::getEndIndxOfStatementList();
	if (!addedProcName) {
		throw "Null node added as child of procedure node";
	}
	while ((tokenIndx) < endIndex) {
		if (peekNextToken().getValue() == "{" ||
			peekNextToken().getValue() == "}") {
			getNextToken();
			continue;
		}
		procNode.AddChild(&parseStatement());
	}

	return procNode;
}

TNode Parser::parseReadStatement() {
	//std::cout << "Parse ReadStatement Called" << std::endl;
	Token varToken = Parser::getNextToken();

	if (varToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid variable name for read statement";
	}

	// insert into VarTable
	
	TNode readNode(TNode::NODE_TYPE::readStmt, statementIndex);
	TNode varNode(TNode::NODE_TYPE::varName, varToken.getValue());
	TNode* varNodePtr = &varNode;

	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ';' in read statement" ;
	}

	if (!readNode.AddChild(varNodePtr)) {
		throw "Null node added as child of read node";
	}

	return readNode;
}

TNode Parser::parsePrintStatement() {
	//std::cout << "Parse PrintStatement Called" << std::endl;
	Token varToken = Parser::getNextToken();
	if (varToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid variable name for print statement";
	}

	TNode printNode(TNode::NODE_TYPE::printStmt, statementIndex);
	TNode varNode(TNode::NODE_TYPE::varName, varToken.getValue());
	TNode* varNodePtr = &varNode;
	// Set value of print node

	if (Parser::getNextToken().getValue() != ";") {
		// throw error for invalid statement
		throw "Missing ; in line: " + statementIndex;
	}

	if (!printNode.AddChild(varNodePtr)) {
		throw "Null node added to child of print node";
	}

	return printNode;
}

TNode Parser::parseCallStatement() {
	//std::cout << "Parse CallStatement Called" << std::endl;
	Token procNameToken = Parser::getNextToken();
	if (procNameToken.getTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid procedure name for call statement";
	}
	
	TNode callNode(TNode::NODE_TYPE::callStmt, statementIndex);
	TNode procNameNode(TNode::NODE_TYPE::procName, statementIndex);
	TNode* procNameNodePtr = &procNameNode;

	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ; in line: " + statementIndex;
	}

	if (!callNode.AddChild(procNameNodePtr)) {
		throw "Null node added to child of call node";
	}
	return callNode;
}

TNode Parser::parseIfStatement() {
	//std::cout << "Parse IfStatement Called" << std::endl;
	TNode ifNode(TNode::NODE_TYPE::ifStmt, statementIndex);
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::IF);

	TNode thenStmtListNode = Parser::parseStatementList();

	TNode* exprNodePtr = &exprNode;
	TNode* thenStmtListNodePtr = &thenStmtListNode;

	if (!ifNode.AddChild(exprNodePtr) || !ifNode.AddChild(thenStmtListNodePtr)) {
		throw "Null node added to child of if node";
	}

	if (peekNextToken().getValue() != "else") {
		std::cout << "HERE" << std::endl;
		return ifNode;
	}

	getNextToken(); // Iterating through 'else' keyword
	TNode elseStmtListNode = Parser::parseStatementList();
	TNode* elseStmtListNodePtr = &elseStmtListNode;

	if (!ifNode.AddChild(elseStmtListNodePtr)) {
		throw "Null node added to child of if node";
	}
	return ifNode;
}

TNode Parser::parseWhileStatement() {
	TNode whleNode(TNode::NODE_TYPE::whileStmt, statementIndex);
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::WHILE);
	TNode stmtListNode = Parser::parseStatementList();
	
	TNode* exprNodePtr = &exprNode;
	TNode* stmtListNodePtr = &stmtListNode;

	if (!whleNode.AddChild(exprNodePtr) || !whleNode.AddChild(stmtListNodePtr)) {
		throw "Null node added to child of while node";
	}

	return whleNode;
}


TNode Parser::parseAssgnStatement(Token nameToken) {
	//std::cout << "Parse AssgnStatement Called" << std::endl;
	TNode assgnNode(TNode::NODE_TYPE::assignStmt,  statementIndex);
	if (Parser::getNextToken().getValue() != "=") {
		throw "Expected '=' token in line " + statementIndex;
	}
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::ASSIGN);
	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ; in line: " + statementIndex;
	}
	TNode* exprNodePtr = &exprNode;

	if (!assgnNode.AddChild(exprNodePtr)) {
		throw "Null node added to child of assign node";
	}

	return assgnNode;
}


// Returns a stmtList node
TNode Parser::parseStatementList() {
	//std::cout << "Parse StatementList Called" << std::endl;
	int endIndex = Parser::getEndIndxOfStatementList();
	TNode stmtListNode(TNode::NODE_TYPE::stmtList, statementIndex);
	// Parse the whole statement block 
	while ((tokenIndx) < endIndex) {
		if (peekNextToken().getValue() == "{" ||
			peekNextToken().getValue() == "}") {
			getNextToken();
			continue;
		}
		//Parser::parseStatement();
		 stmtListNode.AddChild(&parseStatement());
	}


	return stmtListNode;
}

TNode Parser::parseExpressionStatement(Parser::expressionType exprType) {

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

	TNode expressionNode = Parser::parseExpression(exprList);

	return expressionNode;
	
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


TNode Parser::parseExpression(std::vector<Token> exprList) {

	/*for (Token t : exprList) {
		t.print();
	}*/

	ExprEvaluator exprEvaluator(exprList);

	TNode rootNode = exprEvaluator.evaluate();

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
