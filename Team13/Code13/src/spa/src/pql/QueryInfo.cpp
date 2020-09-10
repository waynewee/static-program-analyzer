#include "QueryInfo.h"

/* getters */
unordered_map<RELREF, vector<vector<VAR_NAME>>> QueryInfo::getRelRefMap() {
	return this->relRef_map;
}

unordered_map<VAR_NAME, ENTITY> QueryInfo::getVarMap() {
	return this->var_map;
}

VAR_NAME QueryInfo::getOutputVar() {
	return this->output_var;
}


/* setters */
void QueryInfo::setRelRefMap(unordered_map<RELREF, vector<vector<VAR_NAME>>> relRef_map) {
	this->relRef_map = relRef_map;
}

void QueryInfo::setVarMap(unordered_map<VAR_NAME, ENTITY> var_map) {
	this->var_map = var_map;
}

void QueryInfo::setOutputVar(VAR_NAME output_var) {
	this->output_var = output_var;
}