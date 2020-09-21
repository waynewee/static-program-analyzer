#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP QueryInfo::GetRelRefMap() {
	return this->rel_ref_map_;
}

STRING_STRING_MAP QueryInfo::GetVarMap() {
	return this->var_map_;
}

STRING QueryInfo::GetOutputVar() {
	return this->output_var_;
}

BOOLEAN QueryInfo::IsQueryInfoValid() {
	return this->is_valid_;
}

/* setters */
VOID QueryInfo::SetRelRefMap(STRING_STRINGLISTLIST_MAP rel_ref_map) {
	this->rel_ref_map_ = rel_ref_map;
}

VOID QueryInfo::SetVarMap(STRING_STRING_MAP var_map) {
	this->var_map_ = var_map;
}

VOID QueryInfo::SetOutputVar(STRING output_var) {
	this->output_var_ = output_var;
}

VOID QueryInfo::SetValidToFalse() {
	this->is_valid_ = false;
}

// test print functions
VOID QueryInfo::PrintRelRefMap() {
	cout << "----- RELREF MAP ----- " << endl;
	for (auto const& pair : this->rel_ref_map_) {
		cout << " { " << pair.first << " , ";
		for (vector<string> v : pair.second) {
			cout << " < ";
			for (auto i : v) {
				cout << i << ", ";
			}/*
			else {
				cout << "{ " << *(v->at(0)) << " " << *(v->at(1)) << "} ";
			}*/
		}

	}
}

VOID QueryInfo::PrintVarMap() {
	cout << "----- VAR MAP ----- " << endl;
	for (auto f = var_map_.cbegin(); f != var_map_.cend(); f++) {
		cout << " { " << (*f).first << " , " << (*f).second << " } " << endl;

	}
}

VOID QueryInfo::PrintOutputVar() {
	cout << "---- OUTPUT VAR ---- " << endl;
	cout << "Output variable : " << output_var_ << endl;
}
