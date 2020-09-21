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

	STANDARD_STRINGLIST SplitBySemicolons(string* query);

	string DeleteOneWordAndRetrieveIt(string* str);

	STANDARD_STRING_STRINGLIST_MAP ParsePatternClause(string* clause, STANDARD_STRING_STRING_MAP all_user_declared_var,
		QuerySyntaxValidator* query_syntax_validator);

	STRING_STRING_MAP_PTR ToPointerVarMap(STANDARD_STRING_STRING_MAP str_map);

	STRING_STRINGLISTLIST_MAP_PTR ToPointerRelRefMap(STANDARD_STRING_STRINGLISTLIST_MAP relRef_map);
};

