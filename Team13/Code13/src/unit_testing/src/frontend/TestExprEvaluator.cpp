#include <CustomTypes.h>
#include <frontend/ExprEvaluator.h>
#include <frontend/FrontendTypes.h>
#include <frontend/Token.h>
#include <frontend/Tokenizer.h>
#include <pkb/TNode.h>
#include <testUtils/TreeTraverse.h>

#include "catch.hpp"
#include <vector>
#include <string>
#include <iostream>

Token token_var_x = Token("x", TokenType::TOKEN_TYPE::var);
Token token_const_4 = Token("4", TokenType::TOKEN_TYPE::constant);

Token token_expr_plus = Token(TYPE_EXPR_PLUS, TokenType::TOKEN_TYPE::expr);
Token token_expr_minus = Token(TYPE_EXPR_MINUS, TokenType::TOKEN_TYPE::expr);
Token token_expr_times = Token(TYPE_EXPR_TIMES, TokenType::TOKEN_TYPE::expr);
Token token_expr_divide = Token(TYPE_EXPR_DIVIDE, TokenType::TOKEN_TYPE::expr);

Token token_rel_expr_eq = Token(TYPE_REL_EXPR_EQ, TokenType::TOKEN_TYPE::rel_expr);
Token token_rel_expr_lt = Token(TYPE_REL_EXPR_LT, TokenType::TOKEN_TYPE::rel_expr);
Token token_rel_expr_gte = Token(TYPE_REL_EXPR_GTE, TokenType::TOKEN_TYPE::rel_expr);
Token token_rel_expr_and = Token(TYPE_COND_EXPR_AND, TokenType::TOKEN_TYPE::rel_expr);
Token token_rel_expr_or = Token(TYPE_COND_EXPR_OR, TokenType::TOKEN_TYPE::rel_expr);

Token token_punc_open_paran = Token(TYPE_PUNC_OPEN_PARAN, TokenType::TOKEN_TYPE::punc);
Token token_punc_closed_paran = Token(TYPE_PUNC_CLOSED_PARAN, TokenType::TOKEN_TYPE::punc);

Token token_unary_not = Token(TYPE_COND_EXPR_NOT, TokenType::TOKEN_TYPE::rel_expr, true);
Token token_unary_minus = Token(TYPE_EXPR_MINUS, TokenType::TOKEN_TYPE::expr, true);

TEST_CASE("Expression evaluator | Evaluates rel expression") {

	SECTION("Evaluates equality rel expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_eq);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "RelExpr:7VarName:xConstValue:4";

		REQUIRE(actual == expected);
	}

	SECTION("Evaluates less than rel expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_lt);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "RelExpr:5VarName:xConstValue:4";

		REQUIRE(actual == expected);
	}

	SECTION("Evaluates and rel expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_and);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "RelExpr:1VarName:xConstValue:4";

		REQUIRE(actual == expected);
	}
}


TEST_CASE("Expression evaluator | Evaluates plus/minus expression") {

	SECTION("Evaluates plus expression") {

		TOKEN_LIST token_list;

		token_list.push_back(token_const_4);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_var_x);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:9ConstValue:4VarName:x";

		REQUIRE(actual == expected);
	
	}

	SECTION("Evaluates minus expression") {

		TOKEN_LIST token_list;

		token_list.push_back(token_const_4);
		token_list.push_back(token_expr_minus);
		token_list.push_back(token_var_x);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:10ConstValue:4VarName:x";

		REQUIRE(actual == expected);

	}

}

TEST_CASE("Expression evaluator | Evaluates times/divide expression") {

	SECTION("Evaluates times expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_const_4);
		token_list.push_back(token_expr_times);
		token_list.push_back(token_var_x);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:11ConstValue:4VarName:x";

		REQUIRE(actual == expected);

	}

	SECTION("Evaluates divide expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_const_4);
		token_list.push_back(token_expr_divide);
		token_list.push_back(token_var_x);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:12ConstValue:4VarName:x";

		REQUIRE(actual == expected);
	}

}

TEST_CASE("Expression evaluator | Evaluates expressions containing parantheses") {

	SECTION("Evalutes expression enclosed in parantheses") {
		TOKEN_LIST token_list;

		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:9VarName:xConstValue:4";

		REQUIRE(actual == expected);
	}

	SECTION("Evalutes expression with sub-expression enclosed in parantheses") {
		TOKEN_LIST token_list;

		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:9VarName:xExpr:9VarName:xConstValue:4";

		REQUIRE(actual == expected);
	}

}

TEST_CASE("Expression evaluator | Evaluates expression with unary operators") {

	SECTION("Evaluates expression with unary minus") {
		TOKEN_LIST token_list;

		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_unary_minus);
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "Expr:9Expr:10ConstValue:4VarName:x";

		REQUIRE(actual == expected);
	}

	SECTION("Evaluates expression with unary not") {
		TOKEN_LIST token_list;

		token_list.push_back(token_unary_not);
		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_eq);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* rootNode = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(rootNode);
		string expected = "RelExpr:0RelExpr:7VarName:xConstValue:4";

		REQUIRE(actual == expected);
	}
}

TEST_CASE("Expression evaluator | Evaluates expression with unary operator NOT") {
	TOKEN_LIST token_list;

	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_unary_not);
	token_list.push_back(token_var_x);
	token_list.push_back(token_expr_plus);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);

	ExprEvaluator evaluator(token_list);
	TNode* rootNode = evaluator.Evaluate();
	string actual = TreeTraverse::traverse(rootNode);
	string expected = "Expr:9RelExpr:0ConstValue:4VarName:x";

	REQUIRE(actual == expected);
}

TEST_CASE("Expression evaluator | Evaluates two clause expressions") {
	TOKEN_LIST token_list;

	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_expr_plus);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_rel_expr_and);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_expr_minus);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_punc_closed_paran);

	ExprEvaluator evaluator(token_list);
	TNode* rootNode = evaluator.Evaluate();
	string actual = TreeTraverse::traverse(rootNode);
	string expected = "RelExpr:1Expr:9Expr:10VarName:xConstValue:4VarName:xConstValue:4";

	REQUIRE(actual == expected);
}


TEST_CASE("Expression Evaluator | Evaluates complex expression") {

	TOKEN_LIST token_list;
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_unary_not);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_eq);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_rel_expr_and);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_gte);
	token_list.push_back(token_unary_minus);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_rel_expr_or);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_lt);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_punc_closed_paran);

	ExprEvaluator evaluator(token_list);

	TNode* rootNode = evaluator.Evaluate();

	string actual = TreeTraverse::traverse(rootNode);
	string expected = "RelExpr:2RelExpr:1RelExpr:5RelExpr:0RelExpr:4VarName:xConstValue:4RelExpr:7VarName:xExpr:10VarName:xConstValue:4ConstValue:4";

	REQUIRE(actual == expected);

}