#include "PQLCustomTypes.h"
#include "PQLParserStorage.h"

PQLParserStorage::PQLParserStorage() 
{
	this->var_map_ = STANDARD_STRING_STRING_MAP();
	this->rel_ref_map_ = STANDARD_STRING_STRINGLISTLIST_MAP();

	this->pattern_var_name_ = string();

	this->all_declarations_ = STANDARD_STRINGLIST();

	this->all_user_declared_var_ = STANDARD_STRING_STRING_MAP();

}
/*Getters*/
STANDARD_STRING_STRINGLISTLIST_MAP PQLParserStorage::GetRelRefMap() {
	return this->rel_ref_map_;
}

STANDARD_STRING_STRING_MAP PQLParserStorage::GetVarMap() {
	return this->var_map_;
}
string PQLParserStorage::GetPatternVarName() {
	return this->pattern_var_name_;
}
STANDARD_STRINGLIST PQLParserStorage::GetAllDeclarations() {
	return this->all_declarations_;
}
STANDARD_STRING_STRING_MAP PQLParserStorage::GetAllUserDeclaredVar() {
	return this->all_user_declared_var_;
}

/*Setters*/
void PQLParserStorage::SetRelRefMap(STANDARD_STRING_STRINGLISTLIST_MAP relRef_map) {
	this->rel_ref_map_ = relRef_map;
}
void PQLParserStorage::SetVarMap(STANDARD_STRING_STRING_MAP var_map) {
	this->var_map_ = var_map;
}
void PQLParserStorage::SetPatternVarName(string pattern_var_name) {
	this->pattern_var_name_ = pattern_var_name;
}
void PQLParserStorage::SetAllDeclarations(STANDARD_STRINGLIST all_declarations) {
	this->all_declarations_ = all_declarations;
}
void PQLParserStorage::SetAllUserDeclaredVar(STANDARD_STRING_STRING_MAP all_user_declared_var) {
	this->all_user_declared_var_ = all_user_declared_var;
}

void PQLParserStorage::StoreVariable(pair<string, string> var, string ent_type) {
	this->all_user_declared_var_.insert(var);
	this->var_map_[var.first] = ent_type;
}

void PQLParserStorage::StoreSuchThatClauseResult(STANDARD_STRING_STRINGLIST_MAP result) {
	for (auto const& pair : result) {
		// already inside
		if (this->rel_ref_map_.count(pair.first) == 1) {
			vector<STANDARD_STRINGLIST> empty;
			this->rel_ref_map_.at(pair.first).push_back(pair.second);
		}
		else {
			vector<STANDARD_STRINGLIST> empty;
			this->rel_ref_map_[pair.first] = empty;
			// since second is a vector
			this->rel_ref_map_.at(pair.first).push_back(pair.second);
		}
	}
}

void PQLParserStorage::StorePatternClauseResult(STANDARD_STRING_STRINGLIST_MAP result, string pattern_var_name) {
	for (auto const& pair : result) {
		// already inside
		STANDARD_STRINGLIST p = pair.second;
		p.push_back(pattern_var_name);
		for (auto i : p) {
			//cout << i << " | ";
			//cout << endl;
		}
		if (this->rel_ref_map_.count(pair.first) == 1) {
			vector<STANDARD_STRINGLIST> empty;
			this->rel_ref_map_.at(pair.first).push_back(p);
		}
		else {
			vector<STANDARD_STRINGLIST> empty;
			this->rel_ref_map_[pair.first] = empty;
			// since second is a vector
			this->rel_ref_map_.at(pair.first).push_back(p);
		}
	}
}