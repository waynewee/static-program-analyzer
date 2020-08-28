#include <string>
#include <vector>
#include "tokens/TokenEnum.h"
#include "tokens/Token.h"

class Tokenizer {

public:

	std::vector<std::string> getLines(std::string input) {

	}

	TokenType getTokenType(std::string tokenStr) {

		return TokenType::constant;

	}

	Token createToken(TokenType tokenType, std::string value) {

	}

};