#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP QueryInfo::GetStMap() {
	return this->st_map_;
}

STRING_STRINGLISTLIST_MAP QueryInfo::GetPatternMap() {
	return this->pattern_map_;
}

STRINGPAIR_SET QueryInfo::GetWithMap() {
	return this->with_map_;
}

STRING_STRING_MAP QueryInfo::GetEntityMap() {
	return this->entity_map_;
}

STRING_LIST QueryInfo::GetOutputList() {
	return this->output_list_;
}

BOOLEAN QueryInfo::IsQueryInfoValid() {
	return this->is_valid_;
}

/* setters */
VOID QueryInfo::SetStMap(STRING_STRINGLISTLIST_MAP rel_ref_map) {
	this->st_map_ = rel_ref_map;
}

VOID QueryInfo::SetPatternMap(STRING_STRINGLISTLIST_MAP pattern_map) {
	this->pattern_map_ = pattern_map;
}

VOID QueryInfo::SetWithMap(STRINGPAIR_SET with_map) {
	this->with_map_ = with_map;
}

VOID QueryInfo::SetEntityMap(STRING_STRING_MAP var_map) {
	this->entity_map_ = var_map;
}

VOID QueryInfo::SetOutputList(STRING_LIST output_var) {
	this->output_list_ = output_var;
}

VOID QueryInfo::SetValidToFalse() {
	this->is_valid_ = false;
}

// test print functions
VOID QueryInfo::PrintClausesMap() {
	cout << "----- Clauses ----- " << endl;
	PrintStMap();
	PrintPatternMap();
	PrintWithMap();
	cout << "-------------------- " << endl;
}

VOID QueryInfo::PrintPatternMap() {
	cout << "----- Pattern Clauses ----- " << endl;
	for (auto const& pair : this->pattern_map_) {
		cout << " { " << pair.first << " , ";
		for (STRING_LIST v : pair.second) {
			cout << " < ";
			for (auto i : v) {
				cout << i << ", ";
			}/*
			else {
				cout << "{ " << *(v->at(0)) << " " << *(v->at(1)) << "} ";
			}*/
		}

	}
	cout << "-------------------- " << endl;
}

VOID QueryInfo::PrintStMap() {
	cout << "----- St Clauses ----- " << endl;
	for (auto const& pair : this->st_map_) {
		cout << " { " << pair.first << " , ";
		for (STRING_LIST v : pair.second) {
			cout << " < ";
			for (auto i : v) {
				cout << i << ", ";
			}/*
			else {
				cout << "{ " << *(v->at(0)) << " " << *(v->at(1)) << "} ";
			}*/
		}

	}
	cout << "-------------------- " << endl;
}

VOID QueryInfo::PrintWithMap() {
	cout << "----- With map ----- " << endl;
	for (auto const& pair : this->with_map_) {
		cout << " { " << pair->first << " , ";
		cout << pair->second << endl;

	}
	//cout << "-------------------- " << endl;
}

VOID QueryInfo::PrintEntityMap() {
	cout << "----- ENTITY MAP ----- " << endl;
	for (auto f = entity_map_.cbegin(); f != entity_map_.cend(); f++) {
		cout << " { " << (*f).first << " , " << (*f).second << " } " << endl;
	}
	cout << "-------------------- " << endl;
}

VOID QueryInfo::PrintOutputList() {
	cout << "---- OUTPUT VAR ---- " << endl;
	for (STRING s : output_list_) {
		cout << s << " " << endl;
	}
	cout << "-------------------- " << endl;
}
