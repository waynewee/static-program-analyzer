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
	STRING_STRING_MAP ValidateDeclaration(string decl);

	void ValidateSelectClauseStartsWithSelect(string s);

	bool ValidateVariableName(string s);

	bool ValidateVariableExists(string s, STRING_STRING_MAP varNames);

	STRING_STRINGLIST_MAP ValidateSuchThatClause(string s, STRING_STRING_MAP declaredVarNames);

	STRING_STRINGLIST_MAP ValidatePatternClause(string s, STRING_STRING_MAP declaredVarNames);

	void ValidateExpression(const string& s);

	string TrimLeadingWhitespaces(const string& s);

	string TrimTrailingWhitespaces(const string& s);

	string TrimAllWhitespaces(const string& s);

	bool IsInteger(string i);

	bool IsEntRef(string s, STRING_STRING_MAP declaredVars);

	bool IsOperator(char c);

	bool IsUnderscore(string s);

	int NthOccurrence(string* str, const string& target, int nth);

};

