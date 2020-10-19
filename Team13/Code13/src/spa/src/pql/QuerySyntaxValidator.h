#include "PQLCustomTypes.h"
#include "QueryRules.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;
#pragma once
class QuerySyntaxValidator
{
private:
	QueryRules query_rules_;

public:
	QuerySyntaxValidator();

	BOOLEAN ValidateDeclaration(STRING decl, STRING_STRING_MAP entity_map);

	BOOLEAN ValidateSelect(STRING token);

	BOOLEAN ValidateResultClause(STRING token, STRING_STRING_MAP user_declared_vars);

	BOOLEAN ValidateSuchthatClause(STRING token, STRING_STRING_MAP user_declared_vars);

	STRING GetValidRelRefType(STRING token, STRING_STRING_MAP user_declared_vars);

	BOOLEAN ValidatePatternClause(STRING token, STRING_STRING_MAP user_declared_vars);

	BOOLEAN ValidateWithClause(STRING token, STRING_STRING_MAP user_declared_vars);

};

