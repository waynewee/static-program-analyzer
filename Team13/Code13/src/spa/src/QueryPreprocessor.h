#include <string>
#include <vector>
using namespace std;

#include "DeclarationsParser.h"
#include "SelectClauseParser.h"

#pragma once
class QueryPreprocessor
{
public:

	DeclarationsParser declarations_parser;

	SelectClauseParser select_clause_parser;

	vector<string> declarations;

	vector<string> select_clause_functions; // ?

	void parse(string s);

};

