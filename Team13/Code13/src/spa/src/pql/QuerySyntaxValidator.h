#include "PQLCustomTypes.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;
#pragma once
class QuerySyntaxValidator
{
public:
	STANDARD_STRING_STRING_MAP ValidateDeclaration(string decl);

	void ValidateSelectClauseStartsWithSelect(string s);

	bool ValidateVariableName(string s);

	bool ValidateVariableExists(string s, STANDARD_STRING_STRING_MAP varNames);

	STANDARD_STRING_STRINGLIST_MAP ValidateSuchThatClause(string s, STANDARD_STRING_STRING_MAP declaredVarNames);

	STANDARD_STRING_STRINGLIST_MAP ValidatePatternClause(string s, STANDARD_STRING_STRING_MAP declaredVarNames);

	void ValidateExpression(const string& s);

	string TrimLeadingWhitespaces(const string& s);

	string TrimTrailingWhitespaces(const string& s);

	string TrimAllWhitespaces(const string& s);

	bool IsInteger(string i);

	bool IsEntRef(string s, STANDARD_STRING_STRING_MAP declaredVars);

	bool IsOperator(char c);

	bool IsUnderscore(string s);

	int nthOccurrence(string* str, const string& target, int nth);

};

