#include "PQLCustomTypes.h"
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"
#include <string>
using namespace std;

#pragma once
class PQLParser
{
public:
	QueryInfo* Parse(string s);

	void TrimLeadingWhitespaces(string* s);

	void TrimTrailingWhitespaces(string* s);

	vector<string> SplitBySemicolons(string* query);

	string DeleteOneWordAndRetrieveIt(string* str);

	unordered_map<string, vector<string>> ParsePatternClause(string* clause, unordered_map<string, string> all_user_declared_var, 
		QuerySyntaxValidator* query_syntax_validator);

	STRING_STRING_MAP_PTR ToPointerVarMap(unordered_map<string, string> strMap);

	STRING_STRINGLISTLIST_MAP_PTR ToPointerRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map);
};

