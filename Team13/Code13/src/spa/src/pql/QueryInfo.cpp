#include "QueryInfo.h"

#include <iostream>

/* getters */
unordered_map<string, vector<vector<string>>> QueryInfo::getRelRefMap() {
	return this->relRef_map;
}

unordered_map<VAR_NAME, string> QueryInfo::getVarMap() {
	return this->var_map;
}

string QueryInfo::getOutputVar() {
	return this->output_var;
}

bool QueryInfo::isQueryInfoValid() {
	return this->isValid;
}

/* setters */
void QueryInfo::setRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map) {
	this->relRef_map = relRef_map;
}

void QueryInfo::setVarMap(unordered_map<string, string> var_map) {
	this->var_map = var_map;
}

void QueryInfo::setOutputVar(string output_var) {
	this->output_var = output_var;
}

void QueryInfo::setValidToFalse() {
	this->isValid = false;
}

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
