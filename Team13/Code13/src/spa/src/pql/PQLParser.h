#include <string>
using namespace std;
#include "QueryInfo.h"
#include "QuerySyntaxValidator.h"
#include "WhitespaceHandler.h"
#include "PQLTokenizer.h"
#include <vector>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>

#pragma once
class PQLParser
{
public:
	QueryInfo Parse(STRING query);

	STRING_STRING_MAP ParseDeclaration(STRING decl);

	VOID ParseResultClauseElem(STRING token, STRING_LIST* output_list);

	VOID ParseResultClauseTuple(STRING token, STRING_LIST* output_list);

	STRING_LIST ExtractArguments(STRING token);

	BOOLEAN IsPatternPartial(STRING token);

};

