#include <vector>
#include <stack>
#include <queue>
#include <iostream>

#include "Tokenizer.h"
#include "Parser.h"
#include "TokenType.h"
#include "Token.h"
#include "TNode.h"
#include "Statement.h"
#include "ExprEvaluator.h"

Parser::Parser() {
	currentStatement = Statement();
}

int Parser::parse(std::string input) {

	Tokenizer tokenizer(input);
	tokenizer.tokenize();

	tokenList = tokenizer.getTokenList();
	tokenIndx = -1;
	statementIndex = 0;

	Parser::parseProgram();

	return 0;
}

//NODE Node(NODE_TYPE type, NAME name, VALUE value, OPERATOR op, INDEX line_no, LIST<NODE> children
TNode Parser::parseProgram() {
	while (tokenIndx < (int) tokenList.size() - 1) {
		Parser::parseStatement();
		//Parser::statementList.push_back(Parser::currentStatement);
	}
	TNode rootNode;
	return rootNode;
}

// Returns a statement node
TNode Parser::parseStatement() {
	//std::cout << "Parse Statement Called" << std::endl;
	Token firstToken = Parser::getNextToken();
	statementIndex++;
 	// ADD INTO STATEMENT TABLE
	TNode statementNode;
	switch (firstToken.getTokenType()) {
	case TokenType::TOKEN_TYPE::KEYW:
		switch (firstToken.getKeywordType()) {
		case TokenType::TOKEN_TYPE_KEYW::PROC:
			statementNode = Parser::parseProcStatement();
			break;
		case TokenType::TOKEN_TYPE_KEYW::READ:
			statementNode = Parser::parseReadStatement();
			break;
		case TokenType::TOKEN_TYPE_KEYW::PRNT:
			statementNode = Parser::parsePrntStatement();
			break;
		case TokenType::TOKEN_TYPE_KEYW::CALL:
			statementNode = Parser::parseCallStatement();
			break;
		case TokenType::TOKEN_TYPE_KEYW::IF:
			statementNode = Parser::parseIfStatement();
			break;
		case TokenType::TOKEN_TYPE_KEYW::WHLE:
			statementNode = Parser::parseWhleStatement();
			break;
		default:
			throw "Unhandled keyword";
			break;
		}
		break;
	case TokenType::TOKEN_TYPE::NAME: // For assignment statements
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
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		throw "Invalid procedure name";
	}
	
	TNode procNode(NODE_TYPE::procedure, NULL, NULL, OPERATOR::and, statementIndex);
	TNode procNameNode(NODE_TYPE::procName, nameToken.getValue(), NULL, OPERATOR::and, statementIndex);
	TNode stmtListNode = Parser::parseStatementList();

	TNode* procNameNodePtr = &procNameNode;
	TNode* stmtListNodePtr = &stmtListNode;
	
	bool addedProcName = procNode.AddChild(procNameNodePtr);
	bool addedStmtList = procNode.AddChild(stmtListNodePtr);

	if (!addedProcName || !addedStmtList) {
		throw "Null node added as child of procedure node";
	}
	return procNode;
}

TNode Parser::parseReadStatement() {
	//std::cout << "Parse ReadStatement Called" << std::endl;
	Token varToken = Parser::getNextToken();

	if (varToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		throw "Invalid variable name for read statement";
	}

	// insert into VarTable
	
	TNode readNode(NODE_TYPE::readStmt, NULL, NULL, OPERATOR::and, statementIndex);
	TNode varNode(NODE_TYPE::varName, varToken.getValue(), NULL, OPERATOR::and, statementIndex);
	TNode* varNodePtr = &varNode;

	if (Parser::getNextToken().getValue() != ";") {
		throw "Missing ';' in read statement" ;
	}

	if (!readNode.AddChild(varNodePtr)) {
		throw "Null node added as child of read node";
	}

	return readNode;
}

TNode Parser::parsePrntStatement() {
	//std::cout << "Parse PrintStatement Called" << std::endl;
	Token varToken = Parser::getNextToken();
	if (varToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		throw "Invalid variable name for print statement";
	}

	TNode printNode(NODE_TYPE::printStmt, NULL, NULL, OPERATOR::and, statementIndex);
	TNode varNode(NODE_TYPE::varName, varToken.getValue(), NULL, OPERATOR::and, statementIndex);
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
	if (procNameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		throw "Invalid procedure name for call statement";
	}
	
	TNode callNode(NODE_TYPE::callStmt, NULL, NULL, OPERATOR::and, statementIndex);
	TNode procNameNode(NODE_TYPE::procName, NULL, NULL, OPERATOR::and, statementIndex);
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
	TNode ifNode(NODE_TYPE::ifStmt, NULL, NULL, OPERATOR::and, statementIndex);
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::IF);

	if (getNextToken().getValue() != "then") {
		throw "Missing 'then' on line " + statementIndex;
	}

	TNode thenStmtListNode = Parser::parseStatementList();

	TNode* exprNodePtr = &exprNode;
	TNode* thenStmtListNodePtr = &thenStmtListNode;

	if (!ifNode.AddChild(exprNodePtr) || !ifNode.AddChild(thenStmtListNodePtr)) {
		throw "Null node added to child of if node";
	}

	if (peekNextToken().getValue() != "else") {
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

TNode Parser::parseWhleStatement() {
	TNode whleNode(NODE_TYPE::whileStmt, NULL, NULL, OPERATOR::and, statementIndex);
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
	TNode assgnNode(NODE_TYPE::assignStmt, NULL, NULL, OPERATOR::and, statementIndex);
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
	TNode stmtListNode(NODE_TYPE::stmtList, NULL, NULL, OPERATOR::and, statementIndex);
	// Parse the whole statement block 
	while ((tokenIndx) < endIndex) {
		if (peekNextToken().getValue() == "{" ||
			peekNextToken().getValue() == "}") {
			getNextToken();
			continue;
		}
		Parser::parseStatement();
		// stmtListNode.addChild(parseStatement());
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
		std::cout << "Parsing ASSIGN expression" << std::endl;

		while (Parser::peekNextToken().getValue() != ";") {
			nextToken = Parser::getNextToken();
			exprList.push_back(nextToken);
		}
	}
	else if (exprType == Parser::expressionType::IF || exprType == Parser::expressionType::WHILE) {
		std::cout << "Parsing IF/WHILE expression" << std::endl;

		nextToken = Parser::getNextToken();

		if (nextToken.getValue() != "(") {
			throw "Invalid IF/WHILE expression";
		}

		while (Parser::peekNextToken().getValue() != ")") {
			nextToken = Parser::getNextToken();
			exprList.push_back(nextToken);
		}

		//iterate till end of list
		while (Parser::peekNextToken().getValue() != "{") {
			Parser::getNextToken();
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
