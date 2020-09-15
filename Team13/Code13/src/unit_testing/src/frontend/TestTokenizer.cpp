
#include "frontend/Tokenizer.h"
#include "frontend/Token.h"
#include "frontend/TokenType.h"

#include "catch.hpp"
#include <vector>

TEST_CASE("Tokenizer | Manipulates token string correctly") {

	Tokenizer tokenizer("");

	tokenizer.tokenStr = "procedure";

	SECTION("Resets token string") {
		tokenizer.resetTokenStr();
		REQUIRE(tokenizer.tokenStr.length() == 0);
	}

	SECTION("Adds character to token string") {
		tokenizer.resetTokenStr();
		tokenizer.appendCharToTokenStr('x');
		REQUIRE(tokenizer.tokenStr == "x");
	}

	SECTION("Adds string to token string") {
		tokenizer.resetTokenStr();
		tokenizer.appendStrToTokenStr("x = a + b");
		REQUIRE(tokenizer.tokenStr == "x = a + b");
	}
}

TEST_CASE("Tokenizer | Manipulates tokens correctly") {

	SECTION("Tokenizes string with one keyword token correctly") {
		Tokenizer tokenizer("procedure");
		tokenizer.tokenize();
		Token token = tokenizer.getTokenList().at(0);

		REQUIRE(token.getValue() == "procedure");
		REQUIRE(TokenType::getStmtType(token.getValue()) == TokenType::STMT_TYPE::_procedure);
	}

	SECTION("Tokenizes string with all white spaces correctly") {
		Tokenizer tokenizer("\n \t \r");
		tokenizer.tokenize();
		REQUIRE(tokenizer.getTokenList().size() == 0);
	}

	SECTION("Tokenizes arithmetic expression string correctly") {
		Tokenizer tokenizer("x = a + 5");
		tokenizer.tokenize();
		std::vector<Token> tokenList = tokenizer.getTokenList();

		int i = 0;
		REQUIRE(tokenList.at(i).getValue() == "x");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::var);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "=");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::rel_expr);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "a");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::var);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "+");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::expr);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "5");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::constant);

	}

	SECTION("Tokenizes while statement with one condition correctly") {

		Tokenizer tokenizer("while ( 1 ) {");
		tokenizer.tokenize();
		std::vector<Token> tokenList = tokenizer.getTokenList();

		int i = 0;
		REQUIRE(tokenList.at(i).getValue() == "while");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::stmt);
		REQUIRE(tokenList.at(i).getStmtType() == TokenType::STMT_TYPE::_while);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "(");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::punc);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "1");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::constant);
		i++;
		REQUIRE(tokenList.at(i).getValue() == ")");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::punc);
		i++;
		REQUIRE(tokenList.at(i).getValue() == "{");
		REQUIRE(tokenList.at(i).getTokenType() == TokenType::TOKEN_TYPE::punc);


	}

	SECTION("Tokenizes print statement correctly") {
		Tokenizer tokenizer("print x");
		tokenizer.tokenize();
		std::vector<Token> tokenList = tokenizer.getTokenList();

		REQUIRE(tokenList.at(0).getValue() == "print");
		REQUIRE(tokenList.at(0).getTokenType() == TokenType::TOKEN_TYPE::stmt);
		REQUIRE(tokenList.at(0).getStmtType() == TokenType::STMT_TYPE::_print);

		REQUIRE(tokenList.at(1).getValue() == "x");
		REQUIRE(tokenList.at(1).getTokenType() == TokenType::TOKEN_TYPE::var);
	}

}
