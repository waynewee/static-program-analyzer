#include <string>
using namespace std;
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"

#pragma once
class PQLParser
{
public:
	QueryInfo parse(string s);

	void buildQueryInfo(QueryInfo queryInfo, string s);

	void trimLeadingWhitespaces(string* s);

	void trimTrailingWhitespaces(string* s);

	vector<string> splitBySemicolons(string* query);

	string deleteOneWordAndRetrieveIt(string* str);

	unordered_map<string, vector<string>> parsePatternClause(string* clause, unordered_map<string, string> all_user_declared_var, 
		QuerySyntaxValidator* query_syntax_validator);
};

