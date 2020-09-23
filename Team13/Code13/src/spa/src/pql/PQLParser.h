#include <string>
using namespace std;
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"

#pragma once
class PQLParser
{
public:
	QueryInfo Parse(string s);

	void TrimLeadingWhitespaces(string* s);

	void TrimTrailingWhitespaces(string* s);

	STRING_LIST SplitBySemicolons(string* query);

	string DeleteOneWordAndRetrieveIt(string* str);

	string DeleteByOpenBracketAndRetrieveIt(string* str);

	STRING_STRINGLIST_MAP ParsePatternClause(string* clause, STRING_STRING_MAP all_user_declared_var,
		QuerySyntaxValidator* query_syntax_validator);
};

