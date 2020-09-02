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

	token = "";

	std::vector<Token> tokenList;
}

int Tokenizer::tokenize() {

	std::cout << "Begin Parse" << std::endl;

	while (pos < len - 1) {
		pos += 1;

		char c = text.at(pos);

		//detect whitespace
		if (isWhiteSpace(c)) {
			continue;
			//detect punctuation
		}
		else if (isPunc(c)) {
			appendCharToToken(c);
			addToken(TokenType::TOKEN_TYPE::PUNC);
			resetToken();
			//detect arithmetic operators
		}
		else if (isArop(c)) {
			appendCharToToken(c);
			addToken(TokenType::TOKEN_TYPE::AROP);
			resetToken();
		}
		//detect logical operators
		else if (isLgopPart(c)) {

			appendCharToToken(c);

			/** check if next char forms a logical op string with current char*/
			std::string opStr = "";
			opStr += c;
			opStr += text.at(pos + 1);

			if (isLgop(opStr)) {
				appendCharToToken(text.at(pos + 1));
				pos += 1;
			}

			addToken(TokenType::TOKEN_TYPE::LGOP);
			resetToken();


		}
		else {
			appendCharToToken(c);
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
			token.length() > 0
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
			for (char _c : token) {
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
					if (token == keyword) {
						isKeyword = true;
						break;
					}
				}

				if (isKeyword) {
					Token* keywordToken = addToken(TokenType::TOKEN_TYPE::KEYW);

					keywordToken->keywordType = TokenType::getTokenTypeKeyw(token);
				}
				else {
					addToken(TokenType::TOKEN_TYPE::NAME);
				}

			}

			resetToken();

		}

	}

	printTokenList();

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

void Tokenizer::appendCharToToken(char c) {
	std::string str = "";
	str += c;

	appendStrToToken(str);
}

void Tokenizer::appendStrToToken(std::string str) {
	token += str;
}

void Tokenizer::printToken(std::string type) {
	std::cout << type + "\t" + token << std::endl;
}

Token* Tokenizer::addToken(TokenType::TOKEN_TYPE tokenType) {
	Token token_obj = Token(token, tokenType);

	tokenList.push_back(token_obj);

	return &tokenList.at(tokenList.size()-1);
}

void Tokenizer::resetToken() {
	token = "";
}

void Tokenizer::printTokenList() {

	for (Token token : tokenList) {
		token.print();
	}

}