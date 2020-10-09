#include <string>
using namespace std;
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"

#pragma once
class PQLParser
{
public:
	QueryInfo Parse(STRING query);

	STRING_LIST SplitBySemicolons(STRING* query);

	STRING RetrieveToken(STRING* str);

	STRING RetrieveTokenByOpenBracket(STRING* str);

	STRING RetrieveTokenByClosingAngleBracket(STRING* str);

	STRING_STRINGLIST_MAP ParsePatternClause(string* clause, STRING_STRING_MAP all_user_declared_var,
		QuerySyntaxValidator* query_syntax_validator);

	STRING_STRING_MAP ParseDeclaration(STRING decl);

	VOID ParseResultClauseElem(STRING token, STRING_LIST* output_list);

	VOID ParseResultClauseTuple(STRING token, STRING_LIST* output_list);

	STRING_LIST ExtractArguments(STRING token);

	BOOLEAN IsPatternPartial(STRING token);

};

