#include <SimpleValidator.h>
#include "ValidatorTestCases.h"
#include <Tokenizer.h>
#include <catch.hpp>

TEST_CASE("Testing if SimpleValidator catches cyclic calls") {
	Tokenizer tokenizer(*(cyclic_test_case_1.source));
	SimpleValidator simple_validator;
	REQUIRE(simple_validator.IsValid(tokenizer.GetTokenList()));
}
