#include "QueryPreprocessor.h"
#include "DeclarationsParser.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void QueryPreprocessor::parse(string s) {
	
	string delimiter = "select";
	string entities = s.substr(0, s.find(delimiter));
	string select_clause = s.substr(s.find(delimiter) + 7, s.length());

	cout << "select_clause : " << select_clause << endl;

	// a vector<string> of the declarations
	declarations = declarations_parser.parse(entities);

	//select_clause_parser.parse(declarations, select_clause);

	// for (auto decl : declarations)
	// 	cout << "declarations : " << decl << "\n";
}