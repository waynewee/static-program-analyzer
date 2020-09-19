#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP_PTR QueryInfo::GetRelRefMap() {
	return this->rel_ref_map;
}

STRING_STRING_MAP_PTR QueryInfo::GetVarMap() {
	return this->var_map;
}

STRING_PTR QueryInfo::GetOutputVar() {
	return this->output_var;
}

BOOLEAN QueryInfo::IsQueryInfoValid() {
	return this->is_valid;
}

/* setters */
void QueryInfo::SetRelRefMap(STRING_STRINGLISTLIST_MAP_PTR rel_ref_map) {
	this->rel_ref_map = rel_ref_map;
}

void QueryInfo::SetVarMap(STRING_STRING_MAP_PTR var_map) {
	this->var_map = var_map;
}

void QueryInfo::SetOutputVar(STRING_PTR output_var) {
	this->output_var = output_var;
}

void QueryInfo::SetValidToFalse() {
	this->is_valid = false;
}

// test print functions
void QueryInfo::PrintRelRefMap() {
	cout << "----- RELREF MAP ----- " << endl;
	for (auto f = rel_ref_map->cbegin(); f != rel_ref_map->cend(); f++) {
		STRING_PTR key = (*f).first;
		STRINGLIST_LIST_PTR value = (*f).second;

		cout << *key << ": ";
		
		for (STRING_LIST_PTR v : *value) {
		
			if (v->size() > 2) {
				cout << "{ " << *(v->at(0)) << " " << *(v->at(1)) << " " << *(v->at(2)) << "} ";
			}
			else {
				cout << "{ " << *(v->at(0)) << " " << *(v->at(1)) << "} ";
			}			
		}

	}
}

void QueryInfo::PrintVarMap() {
	cout << "----- VAR MAP ----- " << endl;
	for (auto f = var_map->cbegin(); f != var_map->cend(); f++) {
		cout << " { " << *((*f).first) << " , " << *((*f).second) << " } " << endl;

	}
}

void QueryInfo::PrintOutputVar() {
	cout << "---- OUTPUT VAR ---- " << endl;
	cout << "Output variable : " << *output_var << endl;
}

