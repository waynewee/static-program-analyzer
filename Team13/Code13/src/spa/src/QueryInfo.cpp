#include "QueryInfo.h"

/* getters */
unordered_map<string, vector<vector<string>>> QueryInfo::getRelRefMap() {
	return this->relRef_map;
}

unordered_map<string, string> QueryInfo::getVarMap() {
	return this->var_map;
}

vector<string> QueryInfo::getOutputVar() {
	return this->output_var;
}


/* setters */
void QueryInfo::setRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map) {
	this->relRef_map = relRef_map;
}

void QueryInfo::setVarMap(unordered_map<string, string> var_map) {
	this->var_map = var_map;
}

void QueryInfo::setOutputVar(vector<string> output_var) {
	this->output_var = output_var;
}