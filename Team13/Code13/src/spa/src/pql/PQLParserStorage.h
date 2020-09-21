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
	unordered_map<string, vector<vector<string>>> getRelRefMap();
	unordered_map<string, string> getVarMap();
	string getPatternVarName();
	vector<string> getAllDeclarations();
	unordered_map<string, string> getAllUserDeclaredVar();

	/*Setters*/
	void setRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map);
	void setVarMap(unordered_map<string, string> var_map);
	void setPatternVarName(string pattern_var_name);
	void setAllDeclarations(vector<string> all_declarations);
	void setAllUserDeclaredVar(unordered_map<string, string> all_user_declared_var);

	void storeVariable(pair<string, string> var, string entType);
	void storeSuchThatClauseResult(unordered_map <string, vector<string>> result);
	void storePatternClauseResult(unordered_map <string, vector<string>> result, string pattern_var_name);
};