#include "PQLParserStorage.h"

PQLParserStorage::PQLParserStorage() 
{
	this->all_user_declared_var_ = STRING_STRING_MAP();

}
/*Getters*/
STRING_STRINGLISTLIST_MAP PQLParserStorage::GetRelRefMap() {
	return this->relRef_map_;
}

STRING_STRING_MAP PQLParserStorage::GetVarMap() {
	return this->var_map_;
}
string PQLParserStorage::GetPatternVarName() {
	return this->pattern_var_name_;
}
STRING_LIST PQLParserStorage::GetAllDeclarations() {
	return this->all_declarations_;
}
STRING_STRING_MAP PQLParserStorage::GetAllUserDeclaredVar() {
	return this->all_user_declared_var_;
}

/*Setters*/
void PQLParserStorage::SetRelRefMap(STRING_STRINGLISTLIST_MAP relRef_map) {
	this->relRef_map_ = relRef_map;
}
void PQLParserStorage::SetVarMap(STRING_STRING_MAP var_map) {
	this->var_map_ = var_map;
}
void PQLParserStorage::SetPatternVarName(string pattern_var_name) {
	this->pattern_var_name_ = pattern_var_name;
}
void PQLParserStorage::SetAllDeclarations(STRING_LIST all_declarations) {
	this->all_declarations_ = all_declarations;
}
void PQLParserStorage::SetAllUserDeclaredVar(STRING_STRING_MAP all_user_declared_var) {
	this->all_user_declared_var_ = all_user_declared_var;
}

void PQLParserStorage::StoreVariable(pair<string, string> var, string entType) {
	if (this->var_map_.count(var.first) != 0) {
		// var map already has this variable name... gg
		throw ("Error : Variable has been redeclared, in separate declarations");
	}
	this->all_user_declared_var_.insert(var);
	this->var_map_[var.first] = entType;
}

void PQLParserStorage::StoreSuchThatClauseResult(STRING_STRINGLIST_MAP result) {
	for (auto const& pair : result) {
		// already inside
		if (this->relRef_map_.count(pair.first) == 1) {
			vector<STRING_LIST> empty;
			this->relRef_map_.at(pair.first).push_back(pair.second);
		}
		else {
			vector<STRING_LIST> empty;
			this->relRef_map_[pair.first] = empty;
			// since second is a vector
			this->relRef_map_.at(pair.first).push_back(pair.second);
		}
	}
}

void PQLParserStorage::StorePatternClauseResult(STRING_STRINGLIST_MAP result, string pattern_var_name) {
	for (auto const& pair : result) {
		// already inside
		STRING_LIST p = pair.second;
		p.push_back(pattern_var_name);
		for (auto i : p) {
			//cout << i << " | ";
			//cout << endl;
		}
		if (this->relRef_map_.count(pair.first) == 1) {
			vector<STRING_LIST> empty;
			this->relRef_map_.at(pair.first).push_back(p);
		}
		else {
			vector<STRING_LIST> empty;
			this->relRef_map_[pair.first] = empty;
			// since second is a vector
			this->relRef_map_.at(pair.first).push_back(p);
		}
	}
}