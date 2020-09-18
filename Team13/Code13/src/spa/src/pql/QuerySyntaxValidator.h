#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;
#pragma once
class QuerySyntaxValidator
{
public:
	unordered_map<string, string> ValidateDeclaration(string decl);

	void ValidateSelectClauseStartsWithSelect(string s);

	bool ValidateVariableName(string s);

	bool ValidateVariableExists(string s, unordered_map<string, string> varNames);

	unordered_map<string, vector<string>> ValidateSuchThatClause(string s, unordered_map<string, string> declaredVarNames);

	unordered_map<string, vector<string>> ValidatePatternClause(string s, unordered_map<string, string> declaredVarNames);

	void ValidateExpression(const string& s);

	string TrimLeadingWhitespaces(const string& s);

	string TrimTrailingWhitespaces(const string& s);

	string TrimAllWhitespaces(const string& s);

	bool IsInteger(string i);

	bool IsEntRef(string s, unordered_map<string, string> declaredVars);

	bool IsOperator(char c);

	bool IsUnderscore(string s);

	int nthOccurrence(string* str, const string& target, int nth);

};

