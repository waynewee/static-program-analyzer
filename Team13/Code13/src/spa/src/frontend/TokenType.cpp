#include <stdexcept>
#include <string>

#include <FrontendTypes.h>
#include <CustomTypes.h>
#include <TokenType.h>

using namespace std;

TokenType::STMT_TYPE TokenType::GetStmtType(string tokenStr) {

	if (tokenStr == TYPE_PROC) {
		return TokenType::STMT_TYPE::_procedure;
	}

	if (tokenStr == TYPE_STMT_CALL) {
		return TokenType::STMT_TYPE::_call;
	}

	if (tokenStr == TYPE_STMT_READ) {
		return TokenType::STMT_TYPE::_read;
	}

	if (tokenStr == TYPE_STMT_WHILE) {
		return TokenType::STMT_TYPE::_while;
	}

	if (tokenStr == TYPE_STMT_PRINT) {
		return TokenType::STMT_TYPE::_print;
	}

	if (tokenStr == TYPE_STMT_IF) {
		return TokenType::STMT_TYPE::_if;
	}

	if (tokenStr == TYPE_STMT_IF_THEN) {
		return TokenType::STMT_TYPE::_then;
	}

	if (tokenStr == TYPE_STMT_IF_ELSE) {
		return TokenType::STMT_TYPE::_else;
	}

	throw logic_error("No such token type!");

}

