#include <string>

#include <TokenType.h>

using namespace std;

TokenType::STMT_TYPE TokenType::getStmtType(string tokenStr) {

	if (tokenStr == "procedure") {
		return TokenType::STMT_TYPE::_procedure;
	}

	if (tokenStr == "call") {
		return TokenType::STMT_TYPE::_call;
	}

	if (tokenStr == "read") {
		return TokenType::STMT_TYPE::_read;
	}

	if (tokenStr == "while") {
		return TokenType::STMT_TYPE::_while;
	}

	if (tokenStr == "print") {
		return TokenType::STMT_TYPE::_print;
	}

	if (tokenStr == "if") {
		return TokenType::STMT_TYPE::_if;
	}

	if (tokenStr == "then") {
		return TokenType::STMT_TYPE::_then;
	}

	if (tokenStr == "else") {
		return TokenType::STMT_TYPE::_else;
	}

	throw "No such token type!";

}

