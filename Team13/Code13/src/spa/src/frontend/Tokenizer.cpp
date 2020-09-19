#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "frontend/Token.h"
#include "frontend/TokenType.h"
#include "Tokenizer.h"

using namespace std;

Tokenizer::Tokenizer(string* input) {
	
	pos = -1;
	text = input;
	len = text->length();
	token_str_ = "";

	Tokenize();
}

int Tokenizer::Tokenize() {

	Token prev_token;
	Token* curr_token_ptr;

	while (pos < len - 1) {
		pos += 1;

		char c = text->at(pos);
		int next_post = pos + 1;

		//detect whitespace
		if (IsWhiteSpace(c)) {
			continue;
			//detect punctuation
		}
		else if (IsPunc(c)) {
			AppendCharToTokenStr(c);
			AddToken(TokenType::TOKEN_TYPE::punc);
			ResetTokenStr();
			//detect arithmetic operators
		}
		else if (IsExpr(c)) {
			AppendCharToTokenStr(c);
			prev_token = token_list_.back();
			curr_token_ptr = AddToken(TokenType::TOKEN_TYPE::expr);
			TestAndSetUnary(curr_token_ptr, prev_token);
			ResetTokenStr();
		}
		//detect logical operators
		else if (IsRelExprPart(c)) {

			AppendCharToTokenStr(c);
			
			string op_str = "";
			op_str += c;
			op_str += text->at(next_post);

			if (IsRelExpr(op_str)) {
				AppendCharToTokenStr(text->at(next_post));
				pos += 1;
			}

			prev_token = token_list_.back();
			curr_token_ptr = AddToken(TokenType::TOKEN_TYPE::rel_expr);
			TestAndSetUnary(curr_token_ptr, prev_token);
			ResetTokenStr();


		}
		else {
			AppendCharToTokenStr(c);
		}


		/**
		We are left with
		1. Integers: DIGIT+
		2. Names: LETTER (LETTER|DIGIT)*
		2a. Keywords: Names that match keywords
		where DIGIT: 0-9 && LETTER: A-Z | a-z
		*/

		/**	
		* If we encounter a whitespace on the next char, we evaluate the token
		*/
		
		if (
			token_str_.length() > 0
			&& (next_post >= len
				|| ((
					IsWhiteSpace(text->at(next_post))
					|| IsPunc(text->at(next_post))
					|| IsExpr(text->at(next_post))
					|| IsRelExprPart(text->at(next_post))
					)))) {

			bool is_integer = true;

			//check if integer
			for (char _c : token_str_) {
				if (!isdigit(_c)) {
					is_integer = false;
				}
			}

			if (is_integer) {
				AddToken(TokenType::TOKEN_TYPE::constant);
			}
			else {

				bool is_stmt_name = false;

				//check if keyword
				for (string stmt_name: stmt_names_) {
					if (token_str_ == stmt_name) {
						is_stmt_name = true;
						break;
					}
				}

				if (is_stmt_name) {
					Token* stmtToken = AddToken(TokenType::TOKEN_TYPE::stmt);

					stmtToken->stmt_type_ = TokenType::GetStmtType(token_str_);
				}
				else {
					AddToken(TokenType::TOKEN_TYPE::var);
				}

			}

			ResetTokenStr();

		}

	}

	return 0;

}

bool Tokenizer::IsWhiteSpace(char c) {

	string s = string(1, c);

	return CheckMatch(s, white_spaces_);
}

bool Tokenizer::IsPunc(char c) {
	string s = string(1, c);

	return CheckMatch(s, punctuations_);
}

bool Tokenizer::IsExpr(char c) {
	string s = string(1, c);

	return CheckMatch(s, expressions_);
}

bool Tokenizer::IsRelExprPart(char c) {
	string s = string(1, c);
	return CheckMatch(s, rel_expressions_part_);
}

bool Tokenizer::IsRelExpr(string str) {
	return CheckMatch(str, rel_expressions_);
}

bool Tokenizer::CheckMatch(string s, STR_LIST v) {

	for (string str : v) {
		if (s == str) {
			return true;
		}
	}

	return false;
}

void Tokenizer::AppendCharToTokenStr(char c) {
	string str = "";
	str += c;

	AppendStrToTokenStr(str);
}

void Tokenizer::AppendStrToTokenStr(string str) {
	token_str_ += str;
}


void Tokenizer::TestAndSetUnary(Token* curr_ptr, Token prev) {
	
	if (curr_ptr->GetValue() == TYPE_REL_EXPR_NOT || curr_ptr->GetValue() == TYPE_EXPR_MINUS) {
		if (prev.GetTokenType() == TokenType::TOKEN_TYPE::rel_expr || prev.GetTokenType() == TokenType::TOKEN_TYPE::expr|| prev.GetValue() == TYPE_PUNC_OPEN_PARAN) {
			curr_ptr->is_unary_op_ = true;
		}
	}

	return;
}

Token* Tokenizer::AddToken(TokenType::TOKEN_TYPE token_type) {
	Token token_obj = Token(token_str_, token_type);

	token_list_.push_back(token_obj);

	return &token_list_.at(token_list_.size()-1);
}

void Tokenizer::ResetTokenStr() {
	token_str_ = "";
}

vector<Token> Tokenizer::GetTokenList() {
	return token_list_;
}