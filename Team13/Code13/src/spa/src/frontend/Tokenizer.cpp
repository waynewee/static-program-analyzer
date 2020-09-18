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

	tokenStr = "";

	vector<Token> tokenList;
}

int Tokenizer::Tokenize() {

	Token prevToken;
	Token* currTokenPtr;

	while (pos < len - 1) {
		pos += 1;

		char c = text->at(pos);
		int nextPos = pos + 1;

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
			prevToken = tokenList.back();
			currTokenPtr = AddToken(TokenType::TOKEN_TYPE::expr);
			TestAndSetUnary(currTokenPtr, prevToken);
			ResetTokenStr();
		}
		//detect logical operators
		else if (IsRelExprPart(c)) {

			AppendCharToTokenStr(c);

			/** check if next char forms a logical op string with current char*/
			string opStr = "";
			opStr += c;
			opStr += text->at(nextPos);

			if (IsRelExpr(opStr)) {
				AppendCharToTokenStr(text->at(nextPos));
				pos += 1;
			}

			prevToken = tokenList.back();
			currTokenPtr = AddToken(TokenType::TOKEN_TYPE::rel_expr);
			TestAndSetUnary(currTokenPtr, prevToken);
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
			tokenStr.length() > 0
			&& (nextPos >= len
				|| ((
					IsWhiteSpace(text->at(nextPos))
					|| IsPunc(text->at(nextPos))
					|| IsExpr(text->at(nextPos))
					|| IsRelExprPart(text->at(nextPos))
					)))) {

			bool isInteger = true;

			//check if integer
			for (char _c : tokenStr) {
				if (!isdigit(_c)) {
					isInteger = false;
				}
			}

			if (isInteger) {
				AddToken(TokenType::TOKEN_TYPE::constant);
			}
			else {

				bool isStmtName = false;

				//check if keyword
				for (string stmtName: stmtNames) {
					if (tokenStr == stmtName) {
						isStmtName = true;
						break;
					}
				}

				if (isStmtName) {
					Token* stmtToken = AddToken(TokenType::TOKEN_TYPE::stmt);

					stmtToken->stmtType = TokenType::GetStmtType(tokenStr);
				}
				else {
					AddToken(TokenType::TOKEN_TYPE::var);
				}

			}

			ResetTokenStr();

		}

	}

	//PrintTokenList();

	return 0;

}

bool Tokenizer::IsWhiteSpace(char c) {

	string s = string(1, c);

	return CheckMatch(s, whiteSpaces);
}

bool Tokenizer::IsPunc(char c) {
	string s = string(1, c);

	return CheckMatch(s, punctuations);
}

bool Tokenizer::IsExpr(char c) {
	string s = string(1, c);

	return CheckMatch(s, expressions);
}

bool Tokenizer::IsRelExprPart(char c) {
	string s = string(1, c);
	return CheckMatch(s, rel_expressions_part);
}

bool Tokenizer::IsRelExpr(string str) {
	return CheckMatch(str, rel_expressions);
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
	tokenStr += str;
}

void Tokenizer::PrintToken(string type) {
//	cout << type + "\t" + token << endl;
}

void Tokenizer::TestAndSetUnary(Token* currPtr, Token prev) {
	
	if (currPtr->GetValue() == TYPE_REL_EXPR_NOT || currPtr->GetValue() == TYPE_EXPR_MINUS) {
		if (prev.GetTokenType() == TokenType::TOKEN_TYPE::rel_expr || prev.GetTokenType() == TokenType::TOKEN_TYPE::expr|| prev.GetValue() == TYPE_PUNC_OPEN_PARAN) {
			currPtr->isUnaryOp = true;
		}
	}

	return;
}

Token* Tokenizer::AddToken(TokenType::TOKEN_TYPE tokenType) {
	Token token_obj = Token(tokenStr, tokenType);

	tokenList.push_back(token_obj);

	return &tokenList.at(tokenList.size()-1);
}

void Tokenizer::ResetTokenStr() {
	tokenStr = "";
}

void Tokenizer::PrintTokenList() {

	for (Token token : tokenList) {
		token.Print();
	}

}

vector<Token> Tokenizer::GetTokenList() {
	return tokenList;
}