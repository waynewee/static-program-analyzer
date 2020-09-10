
#include "ExprEvaluator.h"
#include "Token.h"
#include "Tokenizer.h"

#include "catch.hpp"
#include <vector>
#include <string>

TEST_CASE("Expression Evaluator | Evaluates conditional expressions correctly") {

	std::string expr = "(!(x==0) && (x >= -2) || (y<3))";

	Tokenizer tokenizer(expr);
	tokenizer.tokenize();

	ExprEvaluator evaluator(tokenizer.tokenList);

	evaluator.evaluate();
}