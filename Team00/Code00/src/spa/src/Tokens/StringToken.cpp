#include <iostream>
#include <string>
#include "Token.h"

using namespace std;

class StringToken : public Token {

public:
	std::string value;
	StringToken(std::string tokenVal);
};

StringToken::StringToken(std::string tokenVal) {
	cout << "Init String Token" << endl;
	value = tokenVal;
}