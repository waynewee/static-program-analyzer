#include "PQLParserStorage.h"

PQLParserStorage::PQLParserStorage() 
{
	this->all_user_declared_var = unordered_map<string, string>();

}
/*Getters*/
unordered_map<string, vector<vector<string>>> PQLParserStorage::getRelRefMap() {
	return this->relRef_map;
}

unordered_map<string, string> PQLParserStorage::getVarMap() {
	return this->var_map;
}
string PQLParserStorage::getPatternVarName() {
	return this->pattern_var_name;
}
vector<string> PQLParserStorage::getAllDeclarations() {
	return this->all_declarations;
}
unordered_map<string, string> PQLParserStorage::getAllUserDeclaredVar() {
	return this->all_user_declared_var;
}

/*Setters*/
void PQLParserStorage::setRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map) {
	this->relRef_map = relRef_map;
}
void PQLParserStorage::setVarMap(unordered_map<string, string> var_map) {
	this->var_map = var_map;
}
void PQLParserStorage::setPatternVarName(string pattern_var_name) {
	this->pattern_var_name = pattern_var_name;
}
void PQLParserStorage::setAllDeclarations(vector<string> all_declarations) {
	this->all_declarations = all_declarations;
}
void PQLParserStorage::setAllUserDeclaredVar(unordered_map<string, string> all_user_declared_var) {
	this->all_user_declared_var = all_user_declared_var;
}

void PQLParserStorage::storeVariable(pair<string, string> var, string entType) {
	this->all_user_declared_var.insert(var);
	this->var_map[var.first] = entType;
}

void PQLParserStorage::storeSuchThatClauseResult(unordered_map <string, vector<string>> result) {
	for (auto const& pair : result) {
		// already inside
		if (this->relRef_map.count(pair.first) == 1) {
			vector<vector<string>> empty;
			this->relRef_map.at(pair.first).push_back(pair.second);
		}
		else {
			vector<vector<string>> empty;
			this->relRef_map[pair.first] = empty;
			// since second is a vector
			this->relRef_map.at(pair.first).push_back(pair.second);
		}
	}
}

void PQLParserStorage::storePatternClauseResult(unordered_map <string, vector<string>> result, string pattern_var_name) {
	for (auto const& pair : result) {
		// already inside
		vector<string> p = pair.second;
		p.push_back(pattern_var_name);
		for (auto i : p) {
			//cout << i << " | ";
			//cout << endl;
		}
		if (this->relRef_map.count(pair.first) == 1) {
			vector<vector<string>> empty;
			this->relRef_map.at(pair.first).push_back(p);
		}
		else {
			vector<vector<string>> empty;
			this->relRef_map[pair.first] = empty;
			// since second is a vector
			this->relRef_map.at(pair.first).push_back(p);
		}
	}
}