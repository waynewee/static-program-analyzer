#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP QueryInfo::GetRelRefMap() {
	return this->rel_ref_map;
}

STRING_STRING_MAP QueryInfo::GetVarMap() {
	return this->var_map;
}

STRING QueryInfo::GetOutputVar() {
	return this->output_var;
}

BOOLEAN QueryInfo::IsQueryInfoValid() {
	return this->is_valid;
}

/* setters */
VOID QueryInfo::SetRelRefMap(STRING_STRINGLISTLIST_MAP rel_ref_map) {
	this->rel_ref_map = rel_ref_map;
}

VOID QueryInfo::SetVarMap(STRING_STRING_MAP var_map) {
	this->var_map = var_map;
}

VOID QueryInfo::SetOutputVar(STRING output_var) {
	this->output_var = output_var;
}

VOID QueryInfo::SetValidToFalse() {
	this->is_valid = false;
}

// test print functions
VOID QueryInfo::PrintRelRefMap() {
	cout << "----- RELREF MAP ----- " << endl;
	for (auto const& pair : this->rel_ref_map) {
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

VOID QueryInfo::PrintVarMap() {
	cout << "----- VAR MAP ----- " << endl;
	for (auto const& pair : this->var_map) {
		cout << " { " << pair.first << " , " << pair.second << " } " << endl;

	}
}

VOID QueryInfo::PrintOutputVar() {
	cout << "---- OUTPUT VAR ---- " << endl;
	cout << "Output variable : " << this->output_var << endl;
}
