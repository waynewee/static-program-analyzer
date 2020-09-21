#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class PQLParserStorage
{

private:
	STANDARD_STRING_STRING_MAP var_map_;
	STANDARD_STRING_STRINGLISTLIST_MAP rel_ref_map_;

	string pattern_var_name_;

	STANDARD_STRINGLIST all_declarations_;

	STANDARD_STRING_STRING_MAP all_user_declared_var_;

public:
	PQLParserStorage();
	/*Getters*/
	STANDARD_STRING_STRINGLISTLIST_MAP GetRelRefMap();
	STANDARD_STRING_STRING_MAP GetVarMap();
	string GetPatternVarName();
	STANDARD_STRINGLIST GetAllDeclarations();
	STANDARD_STRING_STRING_MAP GetAllUserDeclaredVar();

	/*Setters*/
	void SetRelRefMap(STANDARD_STRING_STRINGLISTLIST_MAP relRef_map);
	void SetVarMap(STANDARD_STRING_STRING_MAP var_map);
	void SetPatternVarName(string pattern_var_name);
	void SetAllDeclarations(STANDARD_STRINGLIST all_declarations);
	void SetAllUserDeclaredVar(STANDARD_STRING_STRING_MAP all_user_declared_var);

	void StoreVariable(pair<string, string> var, string entType);
	void StoreSuchThatClauseResult(STANDARD_STRING_STRINGLIST_MAP result);
	void StorePatternClauseResult(STANDARD_STRING_STRINGLIST_MAP result, string pattern_var_name);
};