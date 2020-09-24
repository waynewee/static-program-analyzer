
#include <frontend/Tokenizer.h>
#include <frontend/Token.h>
#include <frontend/TokenType.h>

#include "catch.hpp"
#include <vector>

bool CompareTokenLists(TOKEN_LIST t_list_a, TOKEN_LIST t_list_b) {

	for (int i = 0; i < t_list_a.size(); i++) {
		Token t_a = t_list_a.at(i);
		Token t_b = t_list_b.at(i);

		if (!t_a.Compare(t_b)) {
			return false;
		}
	};

	return true;
}

TEST_CASE("Tokenizer | Manipulates token string correctly") {

	Tokenizer tokenizer("");

	tokenizer.token_str_ = "procedure";

	SECTION("Resets token string") {
		tokenizer.ResetTokenStr();
		REQUIRE(tokenizer.token_str_.length() == 0);
	}

	SECTION("Adds character to token string") {
		tokenizer.ResetTokenStr();
		tokenizer.AppendCharToTokenStr('x');
		REQUIRE(tokenizer.token_str_ == "x");
	}

	SECTION("Adds string to token string") {
		tokenizer.ResetTokenStr();
		tokenizer.AppendStrToTokenStr("x = a + b");
		REQUIRE(tokenizer.token_str_ == "x = a + b");
	}
}

TEST_CASE("Tokenizer | Manipulates tokens correctly") {

	SECTION("Tokenizes string with one keyword token correctly") {
		Tokenizer tokenizer("procedure");
		tokenizer.Tokenize();
		Token token = tokenizer.GetTokenList().at(0);

		REQUIRE(token.GetValue() == "procedure");
		REQUIRE(TokenType::GetStmtType(token.GetValue()) == TokenType::STMT_TYPE::_procedure);
	}

	SECTION("Tokenizes string with all white spaces correctly") {
		Tokenizer tokenizer("\n \t \r");
		tokenizer.Tokenize();
		REQUIRE(tokenizer.GetTokenList().size() == 0);
	}

	SECTION("Tokenizes arithmetic expression string correctly") {
		Tokenizer tokenizer("x = a + 5");
		tokenizer.Tokenize();
		std::vector<Token> actual_token_list = tokenizer.GetTokenList();
		std::vector<Token> expected_token_list;

		expected_token_list.push_back(Token("x", TokenType::TOKEN_TYPE::var));
		expected_token_list.push_back(Token("=", TokenType::TOKEN_TYPE::assign));
		expected_token_list.push_back(Token("a", TokenType::TOKEN_TYPE::var));
		expected_token_list.push_back(Token("+", TokenType::TOKEN_TYPE::expr));
		expected_token_list.push_back(Token("5", TokenType::TOKEN_TYPE::constant));

		REQUIRE(CompareTokenLists(expected_token_list, actual_token_list));
	}

	SECTION("Tokenizes while statement with one condition correctly") {

		Tokenizer tokenizer("while ( 1 ) {");
		tokenizer.Tokenize();
		std::vector<Token> actual_token_list = tokenizer.GetTokenList();
		std::vector<Token> expected_token_list;

		expected_token_list.push_back(Token("while", TokenType::TOKEN_TYPE::stmt));
		expected_token_list.push_back(Token("(", TokenType::TOKEN_TYPE::punc));
		expected_token_list.push_back(Token("1", TokenType::TOKEN_TYPE::constant));
		expected_token_list.push_back(Token(")", TokenType::TOKEN_TYPE::punc));
		expected_token_list.push_back(Token("{", TokenType::TOKEN_TYPE::punc));

		REQUIRE(CompareTokenLists(expected_token_list, actual_token_list));

	}

	SECTION("Tokenizes print statement correctly") {
		Tokenizer tokenizer("print x");
		tokenizer.Tokenize();
		std::vector<Token> actual_token_list = tokenizer.GetTokenList();
		std::vector<Token> expected_token_list;

		expected_token_list.push_back(Token("print", TokenType::TOKEN_TYPE::stmt));
		expected_token_list.push_back(Token("x", TokenType::TOKEN_TYPE::var));

		REQUIRE(CompareTokenLists(expected_token_list, actual_token_list));
	}

}
