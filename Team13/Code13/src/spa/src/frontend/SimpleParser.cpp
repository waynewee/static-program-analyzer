#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <vector>

#include "ExprEvaluator.h"
#include "SimpleParser.h"
#include <TNode.h>
#include "TokenType.h"
#include "Token.h"

using namespace std;

SimpleParser::SimpleParser() {}

TNode* SimpleParser::Parse(TOKEN_LIST token_list) {

	token_list_ = token_list;
	token_index_ = -1;
	statement_index_ = 0;

	TNode* main_prg = new TNode(TNode::NODE_TYPE::program);
	
	while (token_index_ < (int) token_list_.size() - 1) {
		Token proc_token = SimpleParser::GetNextToken();

		if (proc_token.GetValue() != TYPE_PROC) {
			throw logic_error("Missing procedure");
		}

		TNode* proc_node = SimpleParser::ParseProcStatement();
		main_prg->AddChild(proc_node);
	}

	for (TNode* node : main_prg->GetChildrenVector()) {
		node->Print(node);
	}
	return main_prg;
}

// Returns a statement node
TNode* SimpleParser::ParseStatement() {
	Token first_token = SimpleParser::GetNextToken();
	statement_index_++;

	switch (first_token.GetTokenType()) {
	case TokenType::TOKEN_TYPE::stmt:
		switch (first_token.GetStmtType()) {
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
		return SimpleParser::ParseAssignStatement(first_token);
	default:
		throw logic_error("Unhandled Token: " + first_token.GetValue());
	}
}

TNode* SimpleParser::ParseProcStatement() {
	Token name_token = SimpleParser::GetNextToken();
	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw logic_error("Invalid procedure name");
	}
	
	TNode * proc_node = new TNode(TNode::NODE_TYPE::procedure);
	TNode * proc_name_node = new TNode(TNode::NODE_TYPE::procName, name_token.GetValue());
	TNode * stmt_list_node = SimpleParser::ParseStatementList();

	if (!proc_node->AddChild(proc_name_node) || !proc_node->AddChild((stmt_list_node))) {
		throw logic_error("Null node added to child of proc node");
	}
	
	return proc_node;
}

TNode* SimpleParser::ParseReadStatement() {

	Token var_token = SimpleParser::GetNextToken();

	if (var_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw logic_error("Invalid variable name for read statement");
	}
	
	TNode * read_node = new TNode(TNode::NODE_TYPE::readStmt, statement_index_);
	TNode * var_node = new TNode(TNode::NODE_TYPE::varName, var_token.GetValue());

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw logic_error("Missing ';' in read statement");
	}

	if (!read_node->AddChild(var_node)) {
		throw logic_error("Null node added as child of read node");
	}

	return read_node;
}

TNode* SimpleParser::ParsePrintStatement() {
	Token var_token = SimpleParser::GetNextToken();
	if (var_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		
		throw logic_error("Invalid variable name for print statement");
	}

	TNode * print_node = new TNode (TNode::NODE_TYPE::printStmt, statement_index_);
	TNode * var_node = new TNode(TNode::NODE_TYPE::varName, var_token.GetValue());

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		// throw error for invalid statement
		throw logic_error("Missing ; in line: " + statement_index_);
	}

	if (!print_node->AddChild(var_node)) {
		throw logic_error("Null node added to child of print node");
	}

	return print_node;
}

TNode* SimpleParser::ParseCallStatement() {
	Token proc_name_token = SimpleParser::GetNextToken();
	if (proc_name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		throw logic_error("Invalid procedure name for call statement");
	}
	
	TNode * call_node = new TNode(TNode::NODE_TYPE::callStmt, statement_index_);
	TNode * proc_name_node = new TNode(TNode::NODE_TYPE::procName, statement_index_);

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON){
		throw logic_error("Missing ; in line: " + statement_index_);
	}

	if (!call_node->AddChild(proc_name_node)) {
		throw logic_error("Null node added to child of call node");
	}
	return call_node;
}

TNode* SimpleParser::ParseIfStatement() {
	TNode* if_node = new TNode(TNode::NODE_TYPE::ifStmt, statement_index_);
	TNode* expr_node = SimpleParser::ParseExpressionStatement(SimpleParser::expressionType::_if);
	TNode* thenStmtListNode = SimpleParser::ParseStatementList();

	for (TNode* child : thenStmtListNode->GetChildrenVector()) {
		child->SetParent(if_node);
	}

	if (!if_node->AddChild(expr_node) || !if_node->AddChild(thenStmtListNode)) {
		throw logic_error("Null node added to child of if node");
	}

	if (PeekNextToken().GetValue() != TYPE_STMT_IF_ELSE) {
		throw logic_error("Missing Else Block");
	}

	GetNextToken(); // Iterating through 'else' keyword
	TNode * elseStmtListNode = SimpleParser::ParseStatementList();

	for (TNode* child : elseStmtListNode->GetChildrenVector()) {
		child->SetParent(if_node);
	}

	if (!if_node->AddChild(elseStmtListNode)) {
		throw logic_error("Null node added to child of if node");
	}

	return if_node;
}

TNode* SimpleParser::ParseWhileStatement() {
	TNode* whleNode = new TNode(TNode::NODE_TYPE::whileStmt, statement_index_);
	TNode* expr_node = SimpleParser::ParseExpressionStatement(SimpleParser::expressionType::_while);
	TNode* stmt_list_node = SimpleParser::ParseStatementList();

	for (TNode* child : stmt_list_node->GetChildrenVector()) {
		child->SetParent(whleNode);
	}

	if (!whleNode->AddChild(expr_node) || !whleNode->AddChild(stmt_list_node)) {
		throw logic_error("Null node added to child of while node");
	}

	return whleNode;
}


TNode* SimpleParser::ParseAssignStatement(Token name_token) {
	TNode* assign_node = new TNode(TNode::NODE_TYPE::assignStmt,  statement_index_);
	TNode* name_node = new TNode(TNode::NODE_TYPE::varName, name_token.GetValue());
	if (SimpleParser::GetNextToken().GetValue() != TYPE_EQUALS) {
		throw logic_error("Expected '=' token in line " + statement_index_);
	}

	TNode* expr_node = SimpleParser::ParseExpressionStatement(SimpleParser::expressionType::_assign);

	if (SimpleParser::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		throw logic_error("Missing ; in line: " + statement_index_);
	}
	if (!assign_node->AddChild(name_node) || !assign_node->AddChild(expr_node)) {
		throw logic_error("Null node added to child of assign node");
	}
	return assign_node;
}


// Returns a stmtList node
TNode* SimpleParser::ParseStatementList() {
	int end_index = SimpleParser::GetEndIndxOfStatementList();
	TNode* stmt_list_node = new TNode(TNode::NODE_TYPE::stmtList, statement_index_);
	// Parse the whole statement block 
	while ((token_index_) < end_index) {
		if (PeekNextToken().GetValue() == TYPE_PUNC_OPEN_BRACKET ||
			PeekNextToken().GetValue() == TYPE_PUNC_CLOSED_BRACKET) {
			GetNextToken();
			continue;
		}

		stmt_list_node->AddChild(SimpleParser::ParseStatement());
	}

	return stmt_list_node;
}

TNode* SimpleParser::ParseExpressionStatement(SimpleParser::expressionType exprType) {
	/**
	* An assign expression looks like
	* x + 1;
	*/
	vector<Token> expr_list;

	Token next_token;

	if (exprType == SimpleParser::expressionType::_assign) {

		while (SimpleParser::PeekNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
			next_token = SimpleParser::GetNextToken();
			expr_list.push_back(next_token);
		}
	}
	else if (exprType == SimpleParser::expressionType::_if || exprType == SimpleParser::expressionType::_while) {

		if (SimpleParser::PeekNextToken().GetValue() != TYPE_PUNC_OPEN_PARAN) {
			throw logic_error("Invalid IF/WHILE statement");
		}

		if (exprType == SimpleParser::expressionType::_if) {
			
			while (SimpleParser::PeekNextToken().GetValue() != TYPE_STMT_IF_THEN) {
				next_token = SimpleParser::GetNextToken();
				expr_list.push_back(next_token);
			}

			SimpleParser::GetNextToken();

		}
		else {
			while (SimpleParser::PeekNextToken().GetValue() != TYPE_PUNC_OPEN_BRACKET) {
				next_token = SimpleParser::GetNextToken();
				expr_list.push_back(next_token);
			}
		}
	}
	TNode* expr_node = SimpleParser::ParseExpression(expr_list);
	return expr_node;
	
}

TNode* SimpleParser::ParseExpression(vector<Token> expr_list) {
	ExprEvaluator expr_evaluator(expr_list);
	TNode* root_node = expr_evaluator.Evaluate();
	return root_node;
}

// Returns the number of tokens inside a statement list
int SimpleParser::GetEndIndxOfStatementList() {
	stack <Token> bracket_matcher;
	Token first_token = SimpleParser::PeekNextToken();
	int counter = 1;

	if (first_token.GetValue() == TYPE_PUNC_OPEN_BRACKET) {
		bracket_matcher.push(first_token);
		while (!bracket_matcher.empty()) {
			if (token_index_ + 1 + counter > (int) token_list_.size()) {
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

Token SimpleParser::GetNextToken() {
	token_index_++;
	return token_list_[token_index_];
}

Token SimpleParser::PeekNextToken() {
	return token_list_[token_index_ + 1];
}