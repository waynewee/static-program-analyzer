#include <vector>
#include <algorithm>
#include <iostream>

#include "Statement.h"
#include "Token.h"

Statement::Statement() {}

void printToken(Token t) {
	std::cout << t.getValue() << " ";
}

void Statement::addToken(Token token) {
	Statement::tokensList.push_back(token);
}

void Statement::print() {
	std::cout << "STATEMENT: ";
	std::for_each(Statement::tokensList.begin(), Statement::tokensList.end(), printToken);
	std::cout << std::endl;
}
