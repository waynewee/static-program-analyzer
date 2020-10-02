#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <vector>

#include "SimpleValidator.h"
#include <ExprValidator.h>
#include <TokenType.h>
#include <Token.h>
#include <FrontendTypes.h>

using namespace std;

SimpleValidator::SimpleValidator() {}

bool SimpleValidator::IsValid(TOKEN_LIST token_list) {
	token_list_ = token_list;
	token_index_ = -1;
	statement_index_ = 0;

	if (token_list.size() == 0) {
		throw "Program cannot be empty";
	}

	while (token_index_ < (int) token_list_.size() - 1) {
}		if (SimpleValidator::GetNextToken().GetValue() != "procedure") {
			throw "Missing procedure";
		}

		SimpleValidator::IsValidProc();
	}

bool SimpleValidator::IsValidStmt() {
	Token first_token = SimpleValidator::GetNextToken();
	statement_index_++;

	switch (first_token.GetTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (first_token.GetStmtType()) {
		case TokenType::STMT_TYPE::_read:
			return SimpleValidator::IsValidReadStmt();
		case TokenType::STMT_TYPE::_print:
			return SimpleValidator::IsValidPrintStmt();
		case TokenType::STMT_TYPE::_call:
			return SimpleValidator::IsValidCallStmt();
		case TokenType::STMT_TYPE::_if:
			return SimpleValidator::IsValidIfBlock();
		case TokenType::STMT_TYPE::_while:
			return SimpleValidator::IsValidWhileBlock();
		case TokenType::STMT_TYPE::_procedure:
			throw logic_error("Cannot define a procedure within a procedure");
		default:
			throw logic_error("Unhandled statement name");
		}
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		return SimpleValidator::IsValidAssignment(first_token);
	default:
		throw logic_error("Unhandled Token: " + first_token.GetValue());
	}
}

bool SimpleValidator::IsValidProc() {
	Token name_token = SimpleValidator::GetNextToken();
	
	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid procedure name at line " + statement_index_;
	}

	return SimpleValidator::IsValidStmtList();
}

bool SimpleValidator::IsValidReadStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid variable name for read statement at line " + statement_index_;
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw "Missing ';' at line " + statement_index_;
	}

	return true;
}

bool SimpleValidator::IsValidPrintStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid variable name for print statement at line " + statement_index_;
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw "Missing ';' at line " + statement_index_;
	}

	return true;
}

bool SimpleValidator::IsValidCallStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw "Invalid variable name for call statement at line " + statement_index_;
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw "Missing ';' at line " + statement_index_;
	}

	return true;
}

bool SimpleValidator::IsValidIfBlock() {
	if (SimpleValidator::IsValidExpression(token_list_)) {
		throw "Invalid expression at line " + statement_index_;
	}
	
	if (SimpleValidator::GetNextToken().GetValue() != TYPE_STMT_IF_THEN) {
		throw "Missing 'then' at line " + statement_index_;
	}

	SimpleValidator::IsValidStmtList();

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_STMT_IF_ELSE) {
		throw "Missing 'else' at line " + statement_index_;
	}

	SimpleValidator::IsValidStmtList();
	
	return true;
}

bool SimpleValidator::IsValidWhileBlock() {
	if (SimpleValidator::IsValidExpression(token_list_)) {
		throw "Invalid expression at line " + statement_index_;
	}

	SimpleValidator::IsValidStmtList();
	return true;
}

bool SimpleValidator::IsValidAssignment(Token name_token) {
	if (SimpleValidator::GetNextToken().GetValue() != TYPE_EQUALS) {
		throw "Missing '=' at line " + statement_index_;
	}

	SimpleValidator::IsValidExpression(token_list_);

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw "Missing ';' at line " + statement_index_;
	}
	return true;
}


bool SimpleValidator::IsValidStmtList() {
	int end_index = SimpleValidator::GetEndIndxOfStatementList();
	while ((token_index_) < end_index) {
		if (SimpleValidator::PeekNextToken().GetValue() == TYPE_PUNC_OPEN_BRACKET ||
			SimpleValidator::PeekNextToken().GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
			SimpleValidator::GetNextToken();
			continue;
		}
		SimpleValidator::IsValidStmt();
	}

	return true;
}

bool SimpleValidator::IsValidExpression(TOKEN_LIST expr_list) {
	return ExprValidator::Validate(expr_list);
}

// Returns the number of tokens inside a statement list
int SimpleValidator::GetEndIndxOfStatementList() {
	stack <Token> bracket_matcher;
	Token first_token = SimpleValidator::PeekNextToken();
	int counter = 1;

	if (first_token.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
		bracket_matcher.push(first_token);
		while (!bracket_matcher.empty()) {
			if (token_index_ + 1 + counter > (int)token_list_.size()) {
				// throw Exception for incomplete statement block
				throw logic_error("Incomplete statement block");
			}
			Token next_token = token_list_[token_index_ + 1 + counter];
			if (next_token.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
				bracket_matcher.push(next_token);
			}
			if (next_token.GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
				bracket_matcher.pop();
			}
			counter++;
		}
	}
	return token_index_ + counter;
}

Token SimpleValidator::GetNextToken() {
	token_index_++;

	if (token_index_ < token_list_.size()) {
		return token_list_[token_index_];
	}
	else {
		throw "Out of range.";
	}
}

Token SimpleValidator::PeekNextToken() {
	if (token_index_ + 1 < token_list_.size()) {
		return token_list_[token_index_ + 1];
	}
	else {
		throw "Out of range.";
	}
}


