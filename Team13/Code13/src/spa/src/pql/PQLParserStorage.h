#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class PQLParserStorage
{

private:
	unordered_map<string, string> var_map;
	unordered_map<string, vector<vector<string>>> relRef_map;

	string pattern_var_name;

	vector<string> all_declarations;

	unordered_map<string, string> all_user_declared_var;

public:
	PQLParserStorage();
	/*Getters*/
	unordered_map<string, vector<vector<string>>> GetRelRefMap();
	unordered_map<string, string> GetVarMap();
	string GetPatternVarName();
	vector<string> GetAllDeclarations();
	unordered_map<string, string> GetAllUserDeclaredVar();

	/*Setters*/
	void SetRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map);
	void SetVarMap(unordered_map<string, string> var_map);
	void SetPatternVarName(string pattern_var_name);
	void SetAllDeclarations(vector<string> all_declarations);
	void SetAllUserDeclaredVar(unordered_map<string, string> all_user_declared_var);

	void StoreVariable(pair<string, string> var, string entType);
	void StoreSuchThatClauseResult(unordered_map <string, vector<string>> result);
	void StorePatternClauseResult(unordered_map <string, vector<string>> result, string pattern_var_name);
};