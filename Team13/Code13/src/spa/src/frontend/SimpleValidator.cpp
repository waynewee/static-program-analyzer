#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <stack>
#include <vector>
#include <map>
#include <set>

#include <SimpleValidator.h>
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

	while (token_index_ < (int)token_list_.size() - 1) {
		if (SimpleValidator::GetNextToken().GetValue() != "procedure") {
			cout << "(Line " << statement_index_ << ")";
			throw "Missing procedure";
		}
		SimpleValidator::IsValidProc();
	}

	

	UpdateAdjList();
	// Checking if functioned called exists
	for (pair<string, string> caller_callee_pair : calls_list_) {
		if (proc_adj_list_.count(caller_callee_pair.second) == 0) {
			cout << "(Name: " << caller_callee_pair.second << ") ";
			throw "Undefined procedure called";
		}
	}
	CheckForCyclicCalls();
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
			throw "Cannot define a procedure within a procedure";
		default:
			throw "Unhandled statement name";
		}
	case TokenType::TOKEN_TYPE::var: // For assignment statements
		return SimpleValidator::IsValidAssignment(first_token);
	default:
		cout << "(" << first_token.GetValue() << ") ";
		throw "Unhandled Token" + first_token.GetValue();
	}
}

bool SimpleValidator::IsValidProc() {
	Token name_token = SimpleValidator::GetNextToken();
	if (proc_adj_list_.count(name_token.GetValue()) != 0) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Redefining procedure";
	}
	curr_proc_ = name_token.GetValue();
	proc_adj_list_.insert({name_token.GetValue(), vector<DFS_NODE>()});

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		cout << "(Line: " << statement_index_ << ") ";

		throw "Invalid procedure name";
	}

	return SimpleValidator::IsValidStmtList();
}

bool SimpleValidator::IsValidReadStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid variable name for read statement";
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}

	return true;
}

bool SimpleValidator::IsValidPrintStmt() {
	Token name_token = SimpleValidator::GetNextToken();

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid variable name for print statement";
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}

	return true;
}

bool SimpleValidator::IsValidCallStmt() {
	Token name_token = SimpleValidator::GetNextToken();
	
	calls_list_.push_back(make_pair(curr_proc_, name_token.GetValue()));

	if (name_token.GetTokenType() != TokenType::TOKEN_TYPE::var) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid variable name for call statement";
	}

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
	}

	return true;
}

bool SimpleValidator::IsValidIfBlock() {
	if (!SimpleValidator::IsValidExpression(GetExpressionTokens(ExpressionType::_if))) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid expression in if block";
	}
	
	if (SimpleValidator::GetNextToken().GetValue() != TYPE_STMT_IF_THEN) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing 'then'";
	}

	SimpleValidator::IsValidStmtList();

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_STMT_IF_ELSE) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing 'else'";
	}

	SimpleValidator::IsValidStmtList();
	
	return true;
}

bool SimpleValidator::IsValidWhileBlock() {
	if (!SimpleValidator::IsValidExpression(GetExpressionTokens(ExpressionType::_while))) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Invalid expression in while block";
	}

	SimpleValidator::IsValidStmtList();
	return true;
}

bool SimpleValidator::IsValidAssignment(Token name_token) {
	if (SimpleValidator::GetNextToken().GetValue() != TYPE_EQUALS) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing '='";
	}

	SimpleValidator::IsValidExpression(GetExpressionTokens(_assign));

	if (SimpleValidator::GetNextToken().GetValue() != TYPE_PUNC_SEMICOLON) {
		cout << "(Line: " << statement_index_ << ") ";
		throw "Missing ';'";
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

	cout << "Expression" << endl;

	for (Token token : expr_list) {
		cout << token.GetValue();
	}

	cout << "\nEnd Expression" << endl;

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
				throw "Incomplete statement block";
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

vector<Token> SimpleValidator::GetExpressionTokens(ExpressionType expr_type) {
	vector<Token> expr_list;
	int EndIndexOfTokens = GetEndIndxOfExpression(expr_type);
	//cout << "Tokens List: ";
	while (token_index_ < EndIndexOfTokens) {
		Token next_token = GetNextToken();
		//cout << next_token.GetValue() + " ";
		expr_list.push_back(next_token);
		
	}

	return expr_list;
}

int SimpleValidator::GetEndIndxOfExpression(ExpressionType expr_type) {
	int end_index = token_index_;
	string delimiter;
	if (expr_type == ExpressionType::_if) {
		// End index is before 'then' token
		delimiter = "then";
	}
	else if (expr_type == ExpressionType::_assign) {
		// End index is before ';' token
		delimiter = ";";
	}
	else if (expr_type == ExpressionType::_while) {
		// End index is before '{' token
		delimiter = "{";
	}
	else {
		throw "Unhandled delimiter in SimpleValidator";
	}
	while (token_list_[end_index + 1].GetValue() != delimiter) {
		end_index++;
	}
	return end_index;
}

void SimpleValidator::UpdateAdjList() {
	/*std::cout << "Adj List: " << std::endl;
	for (map<DFS_NODE, vector<DFS_NODE>>::iterator it = proc_adj_list_.begin();
		it != proc_adj_list_.end(); it++)
	{
		std::cout << it->first  // string (key)
			<< std::endl;
	}

	std::cout << "Caller Callee Pair: " << std::endl;
	for (pair<string, string> caller_callee_pair : calls_list_) {
		std::cout << "Caller: " << caller_callee_pair.first
			<< " , Callee: " << caller_callee_pair.second << std::endl;
	}*/
	for (pair<string, string> caller_callee_pair : calls_list_) {
		vector<string> lst = proc_adj_list_.at(caller_callee_pair.first);
		lst.push_back(caller_callee_pair.second);
		proc_adj_list_.at(caller_callee_pair.first) = lst;
	}
}

void SimpleValidator::CheckForCyclicCalls() {
	int node_count = 0;
	set<DFS_NODE> white_set; // Univisited
	set<DFS_NODE> gray_set; // Visiting
	set<DFS_NODE> black_set; // Visited

	map<DFS_NODE, DFS_NODE> parent_map;

	// const bool is_in = container.find(element) != container.end();

	for (map<string, vector<string>>::iterator it = proc_adj_list_.begin(); 
		it != proc_adj_list_.end(); ++it ) { // Adding all nodes to white set
		white_set.insert(it->first);
		node_count++;
	}

	bool cycleDetected = false;
	DFS_NODE curr_node;
	curr_node = *white_set.begin();
	white_set.erase(white_set.begin()); // Popping first item from set
	gray_set.insert(curr_node);
	cout << "Node count: " << node_count << endl;

	while ((black_set.size() != node_count) && !cycleDetected) {
		cout << "Curr node: " << curr_node << endl;
		/*std::cout << "white set: ";
		for (set<DFS_NODE>::iterator it = white_set.begin();
			it != white_set.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl << "gray set: ";
		for (set<DFS_NODE>::iterator it = gray_set.begin();
			it != gray_set.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl << "black set: ";
		for (set<DFS_NODE>::iterator it = black_set.begin();
			it != black_set.end(); it++)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;*/
		vector<DFS_NODE> adj_list = proc_adj_list_.at(curr_node);
		bool node_visited = false;
		for (DFS_NODE node : adj_list) {
			if (white_set.count(node) > 0) { // Node has not been visited
				parent_map.insert({node, curr_node}); // callee, caller
				curr_node = node;
				white_set.erase(white_set.find(node));
				gray_set.insert(node);
				node_visited = true;
				break;
			}
			else if (gray_set.count(node) > 0 ) {
				cycleDetected = true;
				break;
			}
			// Last case is if it's in the black_set, and dunnid to handle so just ignore
		}
		if (!node_visited) {
			// Node has been fully processed, add to black set
			black_set.insert(curr_node);
			if (black_set.size() == node_count) {
				break;
			}
			gray_set.erase(gray_set.find(curr_node));
			curr_node = *(gray_set.rbegin());
		} 
	}
	if (cycleDetected) {
		throw "Recursive call detected";
	}
}
