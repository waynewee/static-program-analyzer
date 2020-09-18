#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;
#pragma once
class QuerySyntaxValidator
{
public:
	unordered_map<string, string> validateDeclaration(string decl);

	void validateSelectClauseStartsWithSelect(string s);

	bool validateVariableName(string s);

	bool validateVariableExists(string s, unordered_map<string, string> varNames);

	unordered_map<string, vector<string>> validateSuchThatClause(string s, unordered_map<string, string> declaredVarNames);

	unordered_map<string, vector<string>> validatePatternClause(string s, unordered_map<string, string> declaredVarNames);

	void validateExpression(const string& s);

	string trimLeadingWhitespaces(const string& s);

	string trimTrailingWhitespaces(const string& s);

	string trimAllWhitespaces(const string& s);

	bool isInteger(string i);

	bool isEntRef(string s, unordered_map<string, string> declaredVars);

	bool isOperator(char c);

	bool isUnderscore(string s);

	int nthOccurrence(string* str, const string& target, int nth);

};

