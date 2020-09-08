#include <vector>
#include <stack>
#include <iostream>

#include "Tokenizer.h"
#include "Parser.h"
#include "TokenType.h"
#include "Token.h"
#include "TNode.h"
#include "Statement.h"

Parser::Parser() {
	currentStatement = Statement();
}

int Parser::parse(std::string input) {

	Tokenizer tokenizer(input);

	tokenizer.tokenize();

	tokenList = tokenizer.getTokenList();
	tokenIndx = -1;

	Parser::parseProgram();

	return 0;
}

//NODE Node(NODE_TYPE type, NAME name, VALUE value, OPERATOR op, INDEX line_no, LIST<NODE> children
TNode Parser::parseProgram() {
	while (tokenIndx < (int) tokenList.size() - 1) {
		Parser::parseStatement();
		Parser::statementList.push_back(Parser::currentStatement);

		//Parser::currentStatement.print();
		//Parser::currentStatement = Statement::Statement();
	}
	TNode rootNode;
	return rootNode;
}

// Returns a statement node
TNode Parser::parseStatement() {
	//std::cout << "Parse Statement Called" << std::endl;
	Token firstToken = Parser::getNextToken();
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
			std::cout << "Unhandled token" << std::endl;

			break;
			// Throw unhandled keyword exception
		}
		break;
	case TokenType::TOKEN_TYPE::NAME: // For assignment statements
		statementNode = Parser::parseAssgnStatement(firstToken);
		break;
	default:
		// Throw unhandled statement exception
		std::cout << "Unhandled Token: " << firstToken.getValue() <<std::endl;
		break;
	}
	//Parser::currentStatement.print();
	Parser::currentStatement = Statement::Statement();

	return statementNode;
}

TNode Parser::parseProcStatement() {
	//std::cout << "Parse ProcStatement Called" << std::endl;
	TNode procNode;
	Token nextToken = Parser::getNextToken();
	if (nextToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception invalid procedure name
		std::cout << "Invalid procedure name" << std::endl;
	}
	std::string procName = nextToken.getValue();
	// SET NAME OF PROC NODE
	TNode stmtListNode = Parser::parseStatementList();
	// Add child node to procNode
	return stmtListNode;
}

TNode Parser::parseReadStatement() {
	//std::cout << "Parse ReadStatement Called" << std::endl;
	TNode readNode;
	Token nameToken = Parser::getNextToken();

	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception, invalid read name
		std::cout << "Invalid variable name for read statement" << std::endl;
	}
	// insert into VarTable
	// Set value of readNode

	if (Parser::getNextToken().getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Missing ;" << std::endl;
	}

	return readNode;
}

TNode Parser::parsePrntStatement() {
	//std::cout << "Parse PrintStatement Called" << std::endl;
	TNode printNode;
	Token nameToken = Parser::getNextToken();
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception, invalid print name
		std::cout << "Invalid variable name for print statement" << std::endl;
	}
	// Set value of print node

	if (Parser::getNextToken().getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Missing ;" << std::endl;
	}

	return printNode;
}

TNode Parser::parseCallStatement() {
	//std::cout << "Parse CallStatement Called" << std::endl;
	TNode callNode;
	Token nameToken = Parser::getNextToken();
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception, invalid call name
		std::cout << "Invalid procedure name for call statement" << std::endl;
	}
	// Set value of call node

	if (Parser::getNextToken().getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Missing ;" << std::endl;
	}

	return callNode;
}

TNode Parser::parseIfStatement() {
	//std::cout << "Parse IfStatement Called" << std::endl;
	TNode ifNode;
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::IF);
	TNode thenStmtListNode = Parser::parseStatementList();
	// ADD EXPR NODE TO CHILD OF IFNODE
	// ADD STMTLIST NODE TO CHILD OF IFNODE
	//std::cout << "Checking if have else" << std::endl;

	if (peekNextToken().getValue() != "else") {
		return ifNode;
	}

	getNextToken();
	TNode elseStmtListNode = Parser::parseStatementList();
	//Add Child for Else Stmt List Node
	return ifNode;
}

TNode Parser::parseWhleStatement() {
	TNode whleNode;
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::WHILE);
	TNode stmtListNode = Parser::parseStatementList();
	// ADD EXPR NODE TO CHILD OF WHLENODE
	// ADD STMTLIST NODE TO CHILD OF WHLENODE
	return whleNode;
}


TNode Parser::parseAssgnStatement(Token nameToken) {
	//std::cout << "Parse AssgnStatement Called" << std::endl;
	TNode assgnNode;
	if (Parser::getNextToken().getValue() != "=") {
		// throw error for invalid assign statement
		std::cout << "Expected '=' token" << std::endl;
	}
	TNode exprNode = Parser::parseExpressionStatement(Parser::expressionType::ASSIGN);
	
	if (Parser::getNextToken().getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Where is my ;" << std::endl;
	}

	// Set value
	return assgnNode;
}


// Returns a stmtList node
TNode Parser::parseStatementList() {
	//std::cout << "Parse StatementList Called" << std::endl;
	int endIndex = Parser::getEndIndxOfStatementList();
	TNode stmtListNode;
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
	std::vector<std::string> exprList;

	TNode expressionNode;
	Token nextToken;

	if (exprType == Parser::expressionType::ASSIGN) {
		std::cout << "Parsing ASSIGN expression" << std::endl;

		while (Parser::peekNextToken().getValue() != ";") {
			nextToken = Parser::getNextToken();
			exprList.push_back(nextToken.getValue());
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
			exprList.push_back(nextToken.getValue());
		}

		//iterate till end of list
		while (Parser::peekNextToken().getValue() != "{") {
			Parser::getNextToken();
		}
	}

	for (std::string s : exprList) {
		std::cout << s << std::endl;
	}

	return expressionNode;
	
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
