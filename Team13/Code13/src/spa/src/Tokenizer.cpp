#include <map>
#include <iostream>
#include <string>
#include <vector>

#include "Tokenizer.h"
#include "Token.h"
#include "TokenType.h"

Tokenizer::Tokenizer(std::string input) {
	cache = {};
	pos = -1;
	text = input;
	len = text.length();

	tokenStr = "";

	std::vector<Token> tokenList;
}

int Tokenizer::tokenize() {

//	std::cout << "Begin Parse" << std::endl;

	Token prevToken;
	Token* currTokenPtr;

	while (pos < len - 1) {
		pos += 1;

		char c = text.at(pos);

		//detect whitespace
		if (isWhiteSpace(c)) {
			continue;
			//detect punctuation
		}
		else if (isPunc(c)) {
			appendCharToTokenStr(c);
			addToken(TokenType::TOKEN_TYPE::PUNC);
			resetTokenStr();
			//detect arithmetic operators
		}
		else if (isArop(c)) {
			appendCharToTokenStr(c);
			prevToken = tokenList.back();
			currTokenPtr = addToken(TokenType::TOKEN_TYPE::AROP);
			testAndSetUnary(currTokenPtr, prevToken);
			resetTokenStr();
		}
		//detect logical operators
		else if (isLgopPart(c)) {

			appendCharToTokenStr(c);

			/** check if next char forms a logical op string with current char*/
			std::string opStr = "";
			opStr += c;
			opStr += text.at(pos + 1);

			if (isLgop(opStr)) {
				appendCharToTokenStr(text.at(pos + 1));
				pos += 1;
			}

			prevToken = tokenList.back();
			currTokenPtr = addToken(TokenType::TOKEN_TYPE::LGOP);
			testAndSetUnary(currTokenPtr, prevToken);
			resetTokenStr();


		}
		else {
			appendCharToTokenStr(c);
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
			&& (pos + 1 >= len
				|| ((
					isWhiteSpace(text.at(pos + 1))
					|| isPunc(text.at(pos + 1))
					|| isArop(text.at(pos + 1))
					|| isLgopPart(text.at(pos + 1))
					)))) {
			//we encountered a whitespace!

			bool isInteger = true;

			//check if integer
			for (char _c : tokenStr) {
				if (!isdigit(_c)) {
					isInteger = false;
				}
			}

			if (isInteger) {
				addToken(TokenType::TOKEN_TYPE::INTR);
			}
			else {

				bool isKeyword = false;

				//check if keyword
				for (std::string keyword : keywords) {
					if (tokenStr == keyword) {
						isKeyword = true;
						break;
					}
				}

				if (isKeyword) {
					Token* keywordToken = addToken(TokenType::TOKEN_TYPE::KEYW);

					keywordToken->keywordType = TokenType::getTokenTypeKeyw(tokenStr);
				}
				else {
					addToken(TokenType::TOKEN_TYPE::NAME);
				}

			}

			resetTokenStr();

		}

	}

	//printTokenList();

	return 0;

}

bool Tokenizer::isWhiteSpace(char c) {
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

bool Tokenizer::isPunc(char c) {
	return (c == '{' || c == '}' || c == ';' || c == '(' || c == ')');
}

bool Tokenizer::isArop(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool Tokenizer::isLgopPart(char c) {
	return (c == '&' || c == '|' || c == '<' || c == '>' || c == '=' || c == '!');
}

bool Tokenizer::isLgop(std::string str) {
	return (str == "&&" || str == "||" || str == "<=" || str == ">=" || str == "==" || str == "!=");
}

void Tokenizer::appendCharToTokenStr(char c) {
	std::string str = "";
	str += c;

	appendStrToTokenStr(str);
}

void Tokenizer::appendStrToTokenStr(std::string str) {
	tokenStr += str;
}

void Tokenizer::printToken(std::string type) {
//	std::cout << type + "\t" + token << std::endl;
}

void Tokenizer::testAndSetUnary(Token* currPtr, Token prev) {
	
	Token curr = *currPtr;

	if (curr.getValue() == "!" || curr.getValue() == "-") {
		if (prev.getTokenType() == TokenType::TOKEN_TYPE::LGOP || prev.getTokenType() == TokenType::TOKEN_TYPE::AROP || prev.getValue() == "(") {
			currPtr->isUnaryOp = true;
		}
	}

	return;
}

Token* Tokenizer::addToken(TokenType::TOKEN_TYPE tokenType) {
	Token token_obj = Token(tokenStr, tokenType);

	tokenList.push_back(token_obj);

	return &tokenList.at(tokenList.size()-1);
}

void Tokenizer::resetTokenStr() {
	tokenStr = "";
}

void Tokenizer::printTokenList() {

	for (Token token : tokenList) {
		token.print();
	}

}

std::vector<Token> Tokenizer::getTokenList() {
	return tokenList;
}