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
#include <string>

using namespace std;

Token token_var_x = Token("x", TokenType::TOKEN_TYPE::var);
Token token_var_y = Token("y", TokenType::TOKEN_TYPE::var);
Token token_const_4 = Token("4", TokenType::TOKEN_TYPE::constant);

Token token_expr_plus = Token(TYPE_EXPR_PLUS, TokenType::TOKEN_TYPE::expr);
Token token_expr_minus = Token(TYPE_EXPR_MINUS, TokenType::TOKEN_TYPE::expr);
Token token_expr_times = Token(TYPE_EXPR_TIMES, TokenType::TOKEN_TYPE::expr);
Token token_expr_divide = Token(TYPE_EXPR_DIVIDE, TokenType::TOKEN_TYPE::expr);

Token token_rel_expr_eq = Token(TYPE_REL_EXPR_EQ, TokenType::TOKEN_TYPE::rel_expr);
Token token_rel_expr_lt = Token(TYPE_REL_EXPR_LT, TokenType::TOKEN_TYPE::rel_expr);
Token token_rel_expr_gte = Token(TYPE_REL_EXPR_GTE, TokenType::TOKEN_TYPE::rel_expr);

Token token_cond_expr_and = Token(TYPE_COND_EXPR_AND, TokenType::TOKEN_TYPE::cond_expr);
Token token_cond_expr_or = Token(TYPE_COND_EXPR_OR, TokenType::TOKEN_TYPE::cond_expr);

Token token_punc_open_paran = Token(TYPE_PUNC_OPEN_PARAN, TokenType::TOKEN_TYPE::punc);
Token token_punc_closed_paran = Token(TYPE_PUNC_CLOSED_PARAN, TokenType::TOKEN_TYPE::punc);

Token token_unary_not = Token(TYPE_COND_EXPR_NOT, TokenType::TOKEN_TYPE::cond_expr, true);

TEST_CASE("Expression evaluator | Evaluates rel expression") {

	SECTION("Evaluates equality rel expression") {
		TOKEN_LIST token_list;

		//x == 4
		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_eq);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::equal);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	}

	SECTION("Evaluates less than rel expression") {
		TOKEN_LIST token_list;

		//x < 4
		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_lt);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::lesser);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	}

}


TEST_CASE("Expression evaluator | Evaluates plus/minus expression") {

	SECTION("Evaluates plus expression") {

		TOKEN_LIST token_list;

		//x+4
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	
	}

	SECTION("Evaluates minus expression") {

		TOKEN_LIST token_list;

		//x-4
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_minus);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::minus);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);


		REQUIRE(actual == expected);

	}

}

TEST_CASE("Expression evaluator | Evaluates times/divide expression") {

	SECTION("Evaluates times expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_times);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);
		
		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::times);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);

	}

	SECTION("Evaluates divide expression") {
		TOKEN_LIST token_list;

		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_divide);
		token_list.push_back(token_const_4);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::divide);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	}

}

TEST_CASE("Expression evaluator | Evaluates expressions containing parantheses") {

	SECTION("Evalutes expression enclosed in parantheses") {
		TOKEN_LIST token_list;

		//(x + 4)
		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	}

	SECTION("Evalutes expression with sub-expression enclosed in parantheses") {
		TOKEN_LIST token_list;

		//(x+(x+4))
		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_expr_plus);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
		TNode* expected_left_node_1 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_1 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_root_node->AddChild(expected_right_node_0);
		expected_right_node_0->AddChild(expected_left_node_1);
		expected_right_node_0->AddChild(expected_right_node_1);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	}

}

TEST_CASE("Expression evaluator | Evaluates expression with unary operators") {

	//!(x == 4)
	SECTION("Evaluates expression with unary not") {
		TOKEN_LIST token_list;

		token_list.push_back(token_unary_not);
		token_list.push_back(token_punc_open_paran);
		token_list.push_back(token_var_x);
		token_list.push_back(token_rel_expr_eq);
		token_list.push_back(token_const_4);
		token_list.push_back(token_punc_closed_paran);

		ExprEvaluator evaluator(token_list);
		TNode* actual_root_node = evaluator.Evaluate();
		string actual = TreeTraverse::traverse(actual_root_node);

		TNode* expected_root_node = new TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::notOp);
		TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::equal);
		TNode* expected_left_node_1 = new TNode(TNode::NODE_TYPE::varName, "x");
		TNode* expected_right_node_1 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

		expected_root_node->AddChild(expected_left_node_0);
		expected_left_node_0->AddChild(expected_left_node_1);
		expected_left_node_0->AddChild(expected_right_node_1);

		string expected = TreeTraverse::traverse(expected_root_node);

		REQUIRE(actual == expected);
	}
}


TEST_CASE("Expression evaluator | Evaluates two clause expressions") {

	//(x == 4)&&(y >= 4)
	TOKEN_LIST token_list;

	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_eq);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_cond_expr_and);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_y);
	token_list.push_back(token_rel_expr_gte);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);

	ExprEvaluator evaluator(token_list);
	TNode* actual_root_node = evaluator.Evaluate();
	string actual = TreeTraverse::traverse(actual_root_node);

	TNode* expected_root_node = new TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::andOp);
	TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::equal);
	TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::geq);
	TNode* expected_left_node_1 = new TNode(TNode::NODE_TYPE::varName, "x");
	TNode* expected_right_node_1 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));
	TNode* expected_left_node_2 = new TNode(TNode::NODE_TYPE::varName, "y");
	TNode* expected_right_node_2 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

	expected_root_node->AddChild(expected_left_node_0);
	expected_root_node->AddChild(expected_right_node_0);
	expected_left_node_0->AddChild(expected_left_node_1);
	expected_left_node_0->AddChild(expected_right_node_1);
	expected_right_node_0->AddChild(expected_left_node_2);
	expected_right_node_0->AddChild(expected_right_node_2);

	string expected = TreeTraverse::traverse(expected_root_node);

	REQUIRE(actual == expected);

}


TEST_CASE("Expression Evaluator | Evaluates complex expression") {

	//(!(x == 4) && (x >= 4) || ( x < 4 ))
	TOKEN_LIST token_list;
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_unary_not);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_eq);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_cond_expr_and);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_gte);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_cond_expr_or);
	token_list.push_back(token_punc_open_paran);
	token_list.push_back(token_var_x);
	token_list.push_back(token_rel_expr_lt);
	token_list.push_back(token_const_4);
	token_list.push_back(token_punc_closed_paran);
	token_list.push_back(token_punc_closed_paran);

	ExprEvaluator evaluator(token_list);

	TNode* actual_root_node = evaluator.Evaluate();

	string actual = TreeTraverse::traverse(actual_root_node);
	
	TNode* expected_root_node = new TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::orOp);
	TNode* expected_left_node_0 = new TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::andOp);
	TNode* expected_right_node_0 = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::lesser);
	TNode* expected_left_node_1 = new TNode(TNode::NODE_TYPE::condExpr, TNode::OPERATOR::notOp);
	TNode* expected_right_node_1 = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::geq);
	TNode* expected_left_node_2 = new TNode(TNode::NODE_TYPE::varName, "x");
	TNode* expected_right_node_2 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));
	TNode* expected_left_node_3 = new TNode(TNode::NODE_TYPE::relExpr, TNode::OPERATOR::equal);
	TNode* expected_left_node_4 = new TNode(TNode::NODE_TYPE::varName, "x");
	TNode* expected_right_node_4 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));
	TNode* expected_left_node_5 = new TNode(TNode::NODE_TYPE::varName, "x");
	TNode* expected_right_node_5 = new TNode(TNode::NODE_TYPE::constValue, stod("4"));

	expected_root_node->AddChild(expected_left_node_0);
	expected_root_node->AddChild(expected_right_node_0);
	expected_left_node_0->AddChild(expected_left_node_1);
	expected_left_node_0->AddChild(expected_right_node_1);
	expected_right_node_0->AddChild(expected_left_node_2);
	expected_right_node_0->AddChild(expected_right_node_2);
	expected_left_node_1->AddChild(expected_left_node_3);
	expected_right_node_2->AddChild(expected_left_node_4);
	expected_right_node_2->AddChild(expected_right_node_4);
	expected_left_node_3->AddChild(expected_left_node_5);
	expected_left_node_3->AddChild(expected_right_node_5);

	string expected = TreeTraverse::traverse(expected_root_node);

	REQUIRE(actual == expected);

}