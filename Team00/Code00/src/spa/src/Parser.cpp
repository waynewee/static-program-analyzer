#include <map>
#include <iostream>
#include <string>
#include <vector>

#include "Parser.h"
#include "Token.h"
#include "Tokenizer.h"

Parser::Parser() {}

int Parser::parse(std::string input) {

	Tokenizer tokenizer(input);

	tokenizer.tokenize();

	return 0;
}