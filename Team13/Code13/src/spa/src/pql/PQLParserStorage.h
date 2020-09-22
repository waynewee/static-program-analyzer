#pragma once

#include <PQLCustomTypes.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class PQLParserStorage
{

private:
	STRING_STRING_MAP var_map_;
	STRING_STRINGLISTLIST_MAP relRef_map_;

	string pattern_var_name_;

	STRING_LIST all_declarations_;

	STRING_STRING_MAP all_user_declared_var_;

public:
	PQLParserStorage();
	/*Getters*/
	STRING_STRINGLISTLIST_MAP GetRelRefMap();
	STRING_STRING_MAP GetVarMap();
	string GetPatternVarName();
	STRING_LIST GetAllDeclarations();
	STRING_STRING_MAP GetAllUserDeclaredVar();

	/*Setters*/
	void SetRelRefMap(STRING_STRINGLISTLIST_MAP relRef_map);
	void SetVarMap(STRING_STRING_MAP var_map);
	void SetPatternVarName(string pattern_var_name);
	void SetAllDeclarations(STRING_LIST all_declarations);
	void SetAllUserDeclaredVar(STRING_STRING_MAP all_user_declared_var);

	void StoreVariable(pair<string, string> var, string entType);
	void StoreSuchThatClauseResult(STRING_STRINGLIST_MAP result);
	void StorePatternClauseResult(STRING_STRINGLIST_MAP result, string pattern_var_name);
};