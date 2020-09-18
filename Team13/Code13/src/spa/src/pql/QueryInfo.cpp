#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP_PTR QueryInfo::getRelRefMap() {
	return this->relRef_map;
}

STRING_STRING_MAP_PTR QueryInfo::getVarMap() {
	return this->var_map;
}

STRING_PTR QueryInfo::getOutputVar() {
	return this->output_var;
}

BOOLEAN QueryInfo::isQueryInfoValid() {
	return this->isValid;
}

/* setters */
void QueryInfo::setRelRefMap(STRING_STRINGLISTLIST_MAP_PTR relRef_map) {
	this->relRef_map = relRef_map;
}

void QueryInfo::setVarMap(STRING_STRING_MAP_PTR var_map) {
	this->var_map = var_map;
}

void QueryInfo::setOutputVar(STRING_PTR output_var) {
	this->output_var = output_var;
}

void QueryInfo::setValidToFalse() {
	this->isValid = false;
}
/*
// test print functions
void QueryInfo::printRelRefMap() {
	cout << "----- RELREF MAP ----- " << endl;
	for (auto const& pair : this->relRef_map) {
		cout << " { " << pair.first << " , ";
		for (vector<string> v : pair.second) {
			cout << " < ";
			for (auto i : v) {
				cout << i << ", ";
			}
			cout << " > ";
		}
		cout << " } " << endl;

	}
}

void QueryInfo::printVarMap() {
	cout << "----- VAR MAP ----- " << endl;
	for (auto const& pair : this->var_map) {
		cout << " { " << pair.first << " , " << pair.second << " } " << endl;

	}
}

void QueryInfo::printOutputVar() {
	cout << "---- OUTPUT VAR ---- " << endl;
	cout << "Output variable : " << this->output_var << endl;
}
*/