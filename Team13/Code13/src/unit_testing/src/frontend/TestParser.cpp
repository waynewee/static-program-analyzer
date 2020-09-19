#include <frontend/SimpleParser.h>
#include <frontend/Tokenizer.h>
#include <testUtils/Statement.h>
#include <frontend/CodeExtractor.h>
#include <catch.hpp>

TEST_CASE("Positive test case: Parsing regular assignment statement") {
	std::string ifStatement = "procedure ifStatementTest {if (x > 5) {call hello;}}";
	Parser parser = Parser();
	parser.parse(ifStatement);
}

