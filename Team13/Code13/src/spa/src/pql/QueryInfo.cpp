#include "QueryInfo.h"

#include <iostream>

/* getters */
STRING_STRINGLISTLIST_MAP QueryInfo::GetStMap() {
	return this->st_map_;
}

STRING_STRINGLISTLIST_MAP QueryInfo::GetPPatternMap() {
	return this->p_pattern_map_;
}

STRING_STRINGLISTLIST_MAP QueryInfo::GetFPatternMap() {
	return this->f_pattern_map_;
}

STRING_STRINGLIST_MAP QueryInfo::GetWithMap() {
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

VOID QueryInfo::SetPPatternMap(STRING_STRINGLISTLIST_MAP p_pattern_map) {
	this->p_pattern_map_ = p_pattern_map;
}

VOID QueryInfo::SetFPatternMap(STRING_STRINGLISTLIST_MAP f_pattern_map) {
	this->f_pattern_map_ = f_pattern_map;
}

VOID QueryInfo::SetWithMap(STRING_STRINGLIST_MAP with_map) {
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
	PrintPPatternMap();
	PrintFPatternMap();
	PrintWithMap();
	cout << "-------------------- " << endl;
}

VOID QueryInfo::PrintFPatternMap() {
	cout << "----- Full Pattern Clauses ----- " << endl;
	for (auto const& pair : this->f_pattern_map_) {
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

VOID QueryInfo::PrintPPatternMap() {
	cout << "----- Partial Pattern Clauses ----- " << endl;
	for (auto const& pair : this->p_pattern_map_) {
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
	cout << "----- St Clauses ----- " << endl;
	for (auto const& pair : this->with_map_) {
		cout << " { " << pair.first << " , ";
		for (STRING v : pair.second) {
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
