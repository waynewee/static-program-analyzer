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

	Parser::parseProgram(tokenizer.getTokenList());

	return 0;
}

//NODE Node(NODE_TYPE type, NAME name, VALUE value, OPERATOR op, INDEX line_no, LIST<NODE> children
TNode Parser::parseProgram(std::vector<Token> tokenList) {
	int tokenIndx = -1;
	int* indxPtr = &tokenIndx;
	while (tokenIndx < (int) tokenList.size() - 1) {
		Parser::parseStatement(indxPtr, tokenList);
		Parser::statementList.push_back(Parser::currentStatement);

		//Parser::currentStatement.print();
		//Parser::currentStatement = Statement::Statement();
	}
	TNode rootNode;
	return rootNode;
}

// Returns a statement node
TNode Parser::parseStatement(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse Statement Called" << std::endl;
	Token firstToken = Parser::getNextToken(tokenIndex, tokenList);
 	// ADD INTO STATEMENT TABLE
	TNode statementNode;
	switch (firstToken.getTokenType()) {
	case TokenType::TOKEN_TYPE::KEYW:
		switch (firstToken.getKeywordType()) {
		case TokenType::TOKEN_TYPE_KEYW::PROC:
			statementNode = Parser::parseProcStatement(tokenIndex, tokenList);
			break;
		case TokenType::TOKEN_TYPE_KEYW::READ:
			statementNode = Parser::parseReadStatement(tokenIndex, tokenList);
			break;
		case TokenType::TOKEN_TYPE_KEYW::PRNT:
			statementNode = Parser::parsePrntStatement(tokenIndex, tokenList);
			break;
		case TokenType::TOKEN_TYPE_KEYW::CALL:
			statementNode = Parser::parseCallStatement(tokenIndex, tokenList);
			break;
		case TokenType::TOKEN_TYPE_KEYW::IF:
			statementNode = Parser::parseIfStatement(tokenIndex, tokenList);
			break;
		case TokenType::TOKEN_TYPE_KEYW::WHLE:
			statementNode = Parser::parseWhleStatement(tokenIndex, tokenList);
			break;
		default:
			std::cout << "Unhandled token" << std::endl;

			break;
			// Throw unhandled keyword exception
		}
		break;
	case TokenType::TOKEN_TYPE::NAME: // For assignment statements
		statementNode = Parser::parseAssgnStatement(firstToken, tokenIndex, tokenList);
		break;
	default:
		// Throw unhandled statement exception
		std::cout << "Unhandled Token: " << firstToken.getValue() <<std::endl;
		break;
	}
	Parser::currentStatement.print();
	Parser::currentStatement = Statement::Statement();

	return statementNode;
}

TNode Parser::parseProcStatement(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse ProcStatement Called" << std::endl;
	TNode procNode;
	Token nextToken = Parser::getNextToken(tokenIndex, tokenList);
	if (nextToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception invalid procedure name
		std::cout << "Invalid procedure name" << std::endl;
	}
	std::string procName = nextToken.getValue();
	// SET NAME OF PROC NODE
	TNode stmtListNode = Parser::parseStatementList(tokenIndex, tokenList);
	// Add child node to procNode
	return stmtListNode;
}

TNode Parser::parseReadStatement(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse ReadStatement Called" << std::endl;
	TNode readNode;
	Token nameToken = Parser::getNextToken(tokenIndex, tokenList);

	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception, invalid read name
		std::cout << "Invalid variable name for read statement" << std::endl;
	}
	// insert into VarTable
	// Set value of readNode

	if (Parser::getNextToken(tokenIndex, tokenList).getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Missing ;" << std::endl;
	}

	return readNode;
}

TNode Parser::parsePrntStatement(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse PrintStatement Called" << std::endl;
	TNode printNode;
	Token nameToken = Parser::getNextToken(tokenIndex, tokenList);
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception, invalid print name
		std::cout << "Invalid variable name for print statement" << std::endl;
	}
	// Set value of print node

	if (Parser::getNextToken(tokenIndex, tokenList).getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Missing ;" << std::endl;
	}

	return printNode;
}

TNode Parser::parseCallStatement(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse CallStatement Called" << std::endl;
	TNode callNode;
	Token nameToken = Parser::getNextToken(tokenIndex, tokenList);
	if (nameToken.getTokenType() != TokenType::TOKEN_TYPE::NAME) {
		// throw exception, invalid call name
		std::cout << "Invalid procedure name for call statement" << std::endl;
	}
	// Set value of call node

	if (Parser::getNextToken(tokenIndex, tokenList).getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Missing ;" << std::endl;
	}

	return callNode;
}

TNode Parser::parseIfStatement(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse IfStatement Called" << std::endl;
	TNode ifNode;
	TNode exprNode = Parser::parseExpression(tokenIndex, tokenList);
	TNode thenStmtListNode = Parser::parseStatementList(tokenIndex, tokenList);
	// ADD EXPR NODE TO CHILD OF IFNODE
	// ADD STMTLIST NODE TO CHILD OF IFNODE
	std::cout << "Checking if have else" << std::endl;

	if (peekNextToken(tokenIndex, tokenList).getValue() != "else") {
		return ifNode;
	}

	getNextToken(tokenIndex, tokenList);
	TNode elseStmtListNode = Parser::parseStatementList(tokenIndex, tokenList);
	//Add Child for Else Stmt List Node
	return ifNode;
}

TNode Parser::parseWhleStatement(int* tokenIndex, std::vector<Token> tokenList) {
	TNode whleNode;
	TNode exprNode = Parser::parseExpression(tokenIndex, tokenList);
	TNode stmtListNode = Parser::parseStatementList(tokenIndex, tokenList);
	// ADD EXPR NODE TO CHILD OF WHLENODE
	// ADD STMTLIST NODE TO CHILD OF WHLENODE
	return whleNode;
}


TNode Parser::parseAssgnStatement(Token nameToken, int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse AssgnStatement Called" << std::endl;
	TNode assgnNode;
	if (Parser::getNextToken(tokenIndex, tokenList).getValue() != "=") {
		// throw error for invalid assign statement
		std::cout << "Expected '=' token" << std::endl;
	}
	TNode exprNode = Parser::parseExpression(tokenIndex, tokenList);
	
	if (Parser::getNextToken(tokenIndex, tokenList).getValue() != ";") {
		// throw error for invalid statement
		std::cout << "Where is my ;" << std::endl;
	}

	// Set value
	return assgnNode;
}


// Returns a stmtList node
TNode Parser::parseStatementList(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse StatementList Called" << std::endl;
	int endIndex = Parser::getEndIndxOfStatementList(tokenIndex, tokenList);
	TNode stmtListNode;
	// Parse the whole statement block 
	while ((*tokenIndex) < endIndex) {
		if (peekNextToken(tokenIndex, tokenList).getValue() == "{" ||
			peekNextToken(tokenIndex, tokenList).getValue() == "}") {
			getNextToken(tokenIndex, tokenList);
			continue;
		}
		Parser::parseStatement(tokenIndex, tokenList);
		// stmtListNode.addChild(parseStatement(tokenIndex, tokenList));
	}

	return stmtListNode;
}

// Returns the root node of an expression
// Only for expressions in the form of x, 5, (NAME LROP NAME/VAR)

TNode Parser::parseExpression(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Parse Expression Called" << std::endl;
	TNode expressionNode;
	Token nextToken = Parser::getNextToken(tokenIndex, tokenList);

	while (true) {
		if (nextToken.getValue() == ";" || nextToken.getValue() == "{") {
			break;
		}
		nextToken = Parser::getNextToken(tokenIndex, tokenList);
	}
	
	(*tokenIndex)--;
	Parser::currentStatement.tokensList.pop_back();
	
	return expressionNode;
}

// Returns the number of tokens inside a statement list
int Parser::getEndIndxOfStatementList(int* tokenIndex, std::vector<Token> tokenList) {
	std::stack <Token> bracketMatcher;
	Token firstToken = Parser::peekNextToken(tokenIndex, tokenList);
	int counter = 1;

	if (firstToken.getValue() == "{") {
		bracketMatcher.push(firstToken);
		while (!bracketMatcher.empty()) {
			if (*(tokenIndex) + 1 + counter > (int) tokenList.size()) {
				// throw Exception for incomplete statement block
				std::cout << "Incomplete statement block" << std::endl;
			}
			Token nextToken = tokenList[*(tokenIndex) + 1 + counter];
			if (nextToken.getValue() == "{") {
				bracketMatcher.push(nextToken);
			}
			if (nextToken.getValue() == "}") {
				bracketMatcher.pop();
			}
			counter++;
		}
	}
	return (*tokenIndex) + counter;
}

Token Parser::getNextToken(int* tokenIndex, std::vector<Token> tokenList) {
	(*tokenIndex)++;
	//std::cout << "Get: ";
	//std::cout << tokenList[(*tokenIndex)].getValue() << std::endl;
	Parser::currentStatement.addToken(tokenList[(*tokenIndex)]); 
	return tokenList[(*tokenIndex)];
}

Token Parser::peekNextToken(int* tokenIndex, std::vector<Token> tokenList) {
	//std::cout << "Peek: ";
	//std::cout << tokenList[(*tokenIndex) + 1].getValue() << std::endl;

	return tokenList[(*tokenIndex) + 1];
}
