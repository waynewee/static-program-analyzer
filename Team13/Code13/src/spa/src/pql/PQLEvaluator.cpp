#include "PQLEvaluator.h"
#include <iostream>

#include "pkb/PKB.h"

QueryResult PQLEvaluator::Evaluate(QueryInfo query_info) {
	cout << "reached" << endl;
	// Final result to be returned
	STRINGLIST_SET final_result_set = *(new STRINGLIST_SET());

	// Expected output list
	STRING_LIST output_list = query_info.GetOutputList();
	BOOLEAN is_boolean_output = IsBooleanOutput(output_list);

	// Unevaluated clauses & constraints
	STRINGSET_STRINGLISTSET_MAP synonyms_map = STRINGSET_STRINGLISTSET_MAP();
	STRINGPAIR_SET constraints = STRINGPAIR_SET();

	// Intermediate results from evaluated clauses
	STRINGLIST_STRINGLISTSET_MAP results_map = STRINGLIST_STRINGLISTSET_MAP();
	STRINGLIST_STRINGLISTSET_MAP final_results_map = STRINGLIST_STRINGLISTSET_MAP();

	// Query Info object
	STRING_STRING_MAP entity_map = query_info.GetEntityMap();

	// Parse clauses conditions
	BOOLEAN has_parsed = true;
	
	if (!ParseClauses(query_info, &constraints)) {
		if (DEBUG) {
			cout << "PQLEvaluator - parsing with clause: failed." << endl;
		}

		has_parsed = false;
	}
	
	if (!ParseClauses(query_info, &synonyms_map)) {
		if (DEBUG) {
			cout << "PQLEvaluator - parsing such that and pattern clause: failed." << endl;
		}

		has_parsed = false;
	}

	if (!has_parsed) {
		return SetResult(is_boolean_output, FALSE, *(new STRINGLIST_SET()));
	}

	// Evaluate no_synonym_set: FALSE -> return empty; ALL TRUE -> continue
	/*for (STRING_LIST* func : no_synonym_set) {
		STRING f_call = (*func)[0];
		STRING param1 = IsVar((*func)[1]) ? "" : (*func)[1];
		STRING param2 = IsVar((*func)[2]) ? "" : (*func)[2];

		if (!EvaluateNoSynonymSet(f_call, param1, param2)) {
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating no synonym clauses: False clause." << endl;
			}

			return SetResult(is_boolean_output, FALSE, *(new STRINGLIST_SET()));
		}
	}*/
	
	// Evaluate synonyms_map: 1 related-synonyms group at a time
	for (auto f = synonyms_map.cbegin(); f != synonyms_map.cend(); f++) {
		STRINGLIST_STRINGLISTSET_MAP tmp_map = results_map;
		STRINGLIST_SET clauses = (*f).second;

		for (STRING_LIST* c : clauses) {
			STRING f_call = (*c)[0];
			STRING param1 = (*c)[1];
			STRING param2 = (*c)[2];

			STRING_LIST c_key = STRING_LIST();
			STRINGLIST_SET c_value = STRINGLIST_SET();
			
			if (f_call.compare(TYPE_COND_PATTERN_F) == 0 || f_call.compare(TYPE_COND_PATTERN_P) == 0) {
				STRING param3 = (*c)[3];
				c_key.push_back(param3);

				if (IsVar(param1)) {
					// first param = synonym
					c_key.push_back(param1);
					c_value = EvaluatePatternCall(f_call, param2, entity_map.at(param3));

					STRINGLIST_SET tmp = EvaluateAllCall(entity_map.at(param3));
					RemoveIrrelevant(&c_value, tmp, 0);

					tmp = EvaluateAllCall(entity_map.at(param1));
					RemoveIrrelevant(&c_value, tmp, 1);
				}
				else {
					// first param != synonym
					c_value = EvaluatePatternCall(f_call, param1, param2, entity_map.at(param3));

					STRINGLIST_SET tmp = EvaluateAllCall(entity_map.at(param3));
					RemoveIrrelevant(&c_value, tmp, 0);
				}
			}
			else {
				if (IsVar(param1) && IsVar(param2)) {
					// both params = synonyms
					c_key.push_back(param1);
					c_key.push_back(param2);
					c_value = EvaluateTwoSynonymSet(f_call);

					STRINGLIST_SET tmp = EvaluateAllCall(entity_map.at(param2));
					RemoveIrrelevant(&c_value, tmp, 0);

					tmp = EvaluateAllCall(entity_map.at(param1));
					RemoveIrrelevant(&c_value, tmp, 1);
				}
				else if (!IsVar(param1) && IsVar(param2)) {
					// first param != synonym & second param = synonym
					// GetXXX() call
					c_key.push_back(param2);
					c_value = EvaluateOneSynonymSet(f_call, param1);

					STRINGLIST_SET tmp = EvaluateAllCall(entity_map.at(param2));
					RemoveIrrelevant(&c_value, tmp, 0);
				}
				else {
					// first param = synonym & second param != synonym
					// GetInverseXXX() call
					c_key.push_back(param1);
					c_value = EvaluateInverseOneSynonymSet(f_call, param2);

					STRINGLIST_SET tmp = EvaluateAllCall(entity_map.at(param1));
					RemoveIrrelevant(&c_value, tmp, 0);
				}
			}

			if (c_value.empty()) {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating synonyms_map: Empty results." << endl;
				}

				return SetResult(is_boolean_output, FALSE, *(new STRINGLIST_SET()));
			}

			// Add into the tmp_map
			AddResult(c_key, c_value, &tmp_map);
		}

		// Check if any of the synonyms is in expected output list
		// or if the constraints not checked: TRUE -> add into results_map; FALSE -> continue
		for (auto tmp_entry = tmp_map.cbegin(); tmp_entry != tmp_map.cend(); tmp_entry++) {
			// Should only have 1 entry
			STRING_LIST* keys = (*tmp_entry).first;
			STRINGLIST_SET values = (*tmp_entry).second;

			// Check constraints
			BOOLEAN is_dependency_checked = CheckConstraints(constraints, entity_map, *keys, &values);

			if (!is_dependency_checked) {
				AddResult(*keys, values, &results_map);
			}
			else {
				if (values.empty()) {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Checked constraints: Empty results." << endl;
					}

					return SetResult(is_boolean_output, FALSE, *(new STRINGLIST_SET()));
				}

				if (IsOutputSynonyms(*keys, output_list)) {
					AddResult(*keys, values, &results_map);
				}
			}			
		}
		
	}

	// Check if output_list is BOOLEAN: TRUE -> return TRUE; FALSE -> continue
	if (is_boolean_output) {
		// Result doesn't matter
		return SetResult(is_boolean_output, TRUE, *(new STRINGLIST_SET()));
	}	

	// Get output_list's results from results_map
	STRING_LIST output_left = output_list;
	for (auto result_entry = results_map.cbegin(); result_entry != results_map.cend(); result_entry++) {
		STRING_LIST* result_key = (*result_entry).first;
		STRING_LIST* key = new STRING_LIST();
		INTEGER_SET index_to_extract = INTEGER_SET();

		for (INTEGER i = 0; i < result_key->size(); i++) {
			for (STRING output_synonym : output_list) {
				if (result_key->at(i).compare(output_synonym) == 0) {
					key->push_back(output_synonym);
					index_to_extract.insert(i);	
					remove(output_left.begin(), output_left.end(), output_synonym);
				}
			}
		}

		if (!key->empty()) {
			// Has >=1 expected output synonym(s)
			STRINGLIST_SET output_result = GetNewResult((*result_entry).second, index_to_extract);
			AddResult(*key, output_result, &final_results_map);
		}
	}

	// Get all output_list's results that are not in results_map
	for (STRING synonym : output_left) {
		STRING_LIST key = *(new STRING_LIST());
		key.push_back(synonym);

		STRINGLIST_SET value = *(new STRINGLIST_SET());
		value = EvaluateAllCall(entity_map.at(synonym));

		AddResult(key, value, &final_results_map);
	}

	// Combine all the expected output results
	final_result_set = GetCombinedResult(final_results_map, output_list);

	/*
	STRING_LIST general_func = ConvertFunction(*func, entity_map);
	STRING_LIST* key_index = IsKey(general_func, one_cache_set);
	if (key_index != nullptr) {
		value = one_cache_set.at(key_index);
	}
	
	// Add into cache
	one_cache_set.insert({ new STRING_LIST(general_func), value });
	
	related_entities.push(Entity(param1, 1));
	related_entities.push(Entity(param2, 1));
	// related_entities[param1]++;
	// related_entities[param2]++;
	*/
	return SetResult(is_boolean_output, TRUE, final_result_set);
}

VOID PQLEvaluator::AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP* results_map) {
	STRING_LIST* check_key = GetRelatedSynonyms(key, *results_map);
	if (check_key == nullptr) {
		// not related (yet)
		results_map->insert({ new STRING_LIST(key), value });
	}
	else {
		// Merge common synonyms & their results
		INTEGERLIST_LIST index_to_compare = GetCommonSynonymsIndex(*check_key, key);
		results_map->at(check_key) = GetCombinedResult(results_map->at(check_key), value, index_to_compare);
	}
}

QueryResult PQLEvaluator::SetResult(BOOLEAN is_boolean_output, STRING bool_result, STRINGLIST_SET result) {
	QueryResult final_result = *(new QueryResult());
	STRINGLIST_SET final_value = *(new STRINGLIST_SET());

	if (is_boolean_output) {
		STRING_LIST* value = new STRING_LIST();
		value->push_back(bool_result);
		final_value.insert(value);
	}
	else {
		final_value = result;
	}

	final_result.SetResult(final_value);
	return final_result;
}

BOOLEAN PQLEvaluator::ParseClauses(QueryInfo query_info, STRINGPAIR_SET* constraints) {
	STRINGPAIR_SET with_map = query_info.GetWithMap();

	for (STRING_PAIR* clause: with_map) {
		STRING lhs = clause->first;
		STRING rhs = clause->second;

		if (!IsVar(lhs)) {
			// INT=INT or STRING=STRING
			// Check if it is equal
			// If FALSE -> return false, else continue
			if (lhs.compare(rhs) != 0) {
				return false;
			}
		}
		else if (!(IsVar(rhs) && lhs.compare(rhs) == 0)) {
			// synonym=synonym: different synonyms
			// or synonym=INT or synonym=STRING
			if (constraints->insert(clause).second == 0) {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Parsing clauses: Error inserting constraints." << endl;
				}
				return false;
			}
		}
	}

	return true;
}

BOOLEAN PQLEvaluator::ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map) {
	STRING_STRINGLISTLIST_MAP st_map = query_info.GetStMap();
	STRING_STRINGLISTLIST_MAP pattern_map = query_info.GetPatternMap();
	// STRING_INTEGER_MAP occurrence_count = *(new STRING_INTEGER_MAP());

	for (auto f = st_map.cbegin(); f != st_map.cend(); f++) {
		STRING f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			if (p[0].compare(p[1]) == 0) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating such that clauses: Same parameters." << endl;
				}
				return false;
			}

			STRING_LIST* clause = new STRING_LIST();
			clause->push_back(f_call);
			clause->push_back(p[0]);
			clause->push_back(p[1]);

			if (clause->empty()) {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Parsing such that clauses: Error creating value for such that clauses." << endl;
				}

				return false;
			}

			if (!IsVar(p[0]) && !IsVar(p[1])) {
				// both params != synonyms
				STRING param1 = IsVar(clause->at(1)) ? "" : clause->at(1);
				STRING param2 = IsVar(clause->at(2)) ? "" : clause->at(2);

				if (!EvaluateNoSynonymSet(f_call, param1, param2)) {
					if (DEBUG) {
						cout << "PQLEvaluator - Evaluating no synonym clauses: False clause." << endl;
					}

					return false;
				}

				/*if (no_synonym_set->insert(clause).second == 0) {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Parsing clauses: Error inserting no_synonym_set." << endl;
					}

					return false;
				}*/
			}
			else {
				STRING_SET* key = new STRING_SET();
				STRING_SET* check_key = new STRING_SET();
				STRINGLIST_SET value = *(new STRINGLIST_SET());

				if (IsVar(p[0]) && !IsVar(p[1])) {
					// 1st param == synonym
					// occurrence_count[p[0]] += 1;
					key->insert(p[0]);
					check_key = GetRelatedSynonyms(p[0], *synonyms_map);
				}
				else if (!IsVar(p[0]) && IsVar(p[1])) {
					// 2nd param == synonym
					key->insert(p[1]);
					check_key = GetRelatedSynonyms(p[1], *synonyms_map);
				}
				else if (IsVar(p[0]) && IsVar(p[1])) {
					// 2 params = synonyms
					key->insert(p[0]);
					key->insert(p[1]);
					check_key = GetRelatedSynonyms(*key, synonyms_map);
				}
				else {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Parsing such that clauses: Invalid parameters." << endl;
					}

					return false;
				}

				if (check_key != nullptr) {
					key = check_key;
					value = synonyms_map->at(key);
				}

				value.insert(clause);

				if (synonyms_map->insert({ key, value }).second == 0) {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Parsing such that clauses: Error inserting into synonyms_map." << endl;
					}
					return false;
				}
			}
		}
	}

	for (auto f = pattern_map.cbegin(); f != pattern_map.cend(); f++) {
		STRING f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			STRING_LIST* clause = new STRING_LIST();
			clause->push_back(f_call);
			clause->push_back(p[0]);
			clause->push_back(p[1]);
			clause->push_back(p[2]);

			if (clause->empty()) {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Parsing clauses: Error creating value for pattern clauses." << endl;
				}

				return false;
			}

			STRING_SET* key = new STRING_SET();
			key->insert(p[2]);

			STRING_SET* check_key = new STRING_SET();
			STRINGLIST_SET value = *(new STRINGLIST_SET());

			if (IsVar(p[0])) {
				// 1st param = synonym 
				key->insert(p[0]);
				check_key = GetRelatedSynonyms(*key, synonyms_map);
			}
			else {
				// both params != synonyms
				check_key = GetRelatedSynonyms(p[2], *synonyms_map);
			}

			if (check_key != nullptr) {
				key = check_key;
				value = synonyms_map->at(key);
			}

			value.insert(clause);

			if (synonyms_map->insert({ key, value }).second == 0) {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Parsing pattern clauses: Error inserting into synonyms_map." << endl;
				}
				return false;
			}
		}
	}

	return true;
}

STRING_LIST* PQLEvaluator::GetRelatedSynonyms(STRING synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		for (STRING s: *existing_key) {
			// found related synonyms in the synonyms_map
			if (s.compare(synonym) == 0) {
				return existing_key;
			}
		}
	}

	return nullptr;
}

STRING_LIST* PQLEvaluator::GetRelatedSynonyms(STRING_LIST synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		for (STRING existing_synonym: *existing_key) {
			// found related synonyms in the synonyms_map
			for (STRING checking_synonym: synonym) {
				if (existing_synonym.compare(checking_synonym) == 0) {
					return existing_key;
				}
			}
		}
	}

	return nullptr;
}

STRING_SET* PQLEvaluator::GetRelatedSynonyms(STRING synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map) {
	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_SET* existing_key = (*k).first;

		if (existing_key->find(synonym) != existing_key->end()) {
			// found related synonyms in the synonyms_map
			return existing_key;
		}
	}

	return nullptr;
}

STRING_SET* PQLEvaluator::GetRelatedSynonyms(STRING_SET synonyms, STRINGSET_STRINGLISTSET_MAP* synonyms_map) {
	BOOLEAN has_related_synonyms = false;

	STRING_SET* new_keys = new STRING_SET();
	STRINGLIST_SET new_values = *(new STRINGLIST_SET());

	for (auto k = synonyms.cbegin(); k != synonyms.cend(); k++) {
		STRING curr_check = *k;
		STRING_SET* check_key = GetRelatedSynonyms(curr_check, *synonyms_map);
		if (check_key != nullptr) {
			// found related synonyms in the synonyms_map
			new_keys->insert(check_key->begin(), check_key->end());

			// combine the clauses
			new_values.insert(synonyms_map->at(check_key).begin(), synonyms_map->at(check_key).end());

			// remove the existing synonyms since already combined
			synonyms_map->erase(check_key);
		}
	}

	if (!new_keys->empty()) {
		// make sure none is missed out
		new_keys->insert(synonyms.begin(), synonyms.end());

		// insert the combined clauses into the new combined synonyms
		synonyms_map->insert({ new_keys, new_values });

		return new_keys;
	}
	else {
		return nullptr;
	}
}

BOOLEAN PQLEvaluator::EvaluateNoSynonymSet(STRING f_call, STRING param1, STRING param2) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateNoSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param1: " << param1 << "; param2: " << param2 << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateNoSynonymSet - UNIT TESTING" << endl;
		return true;
	}

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		return rm.IsFollows(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		return rm.IsFollowsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		return rm.IsParent(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		return rm.IsParentStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		return rm.IsStmtUses(ParsingStmtRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		return rm.IsProcUses(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		return rm.IsStmtModifies(ParsingStmtRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		return rm.IsProcModifies(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		//return rm.IsNext(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		//return rm.IsNextStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		//return rm.IsAffects(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		//return rm.IsAffectsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateNoSynonymSet: No such relref." << endl;
		}

		return false;
	}
}

STRINGLIST_SET PQLEvaluator::EvaluatePatternCall(STRING f_call, STRING param1, STRING param2, STRING type) {
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluatePatternCall (2 params to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param1 << "; param2: " << param2 << "; type: " << type << endl;
	}

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			// result = ConvertSet(pm.GetIfWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			// result = ConvertSet(pm.GetWhileWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			// result = ConvertSet(pm.GetIfWithFullPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			// result = ConvertSet(pm.GetWhileWithFullPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithFullPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluatePatternCall(STRING f_call, STRING param, STRING type) {
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall (1 param to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param << "; type: " << type << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall (1 param to pass) - UNIT TESTING" << endl;

		STRING_LIST* val1 = new STRING_LIST();
		val1->push_back("1");
		val1->push_back("v1");
		STRING_LIST* val2 = new STRING_LIST();
		val2->push_back("2");
		val2->push_back("v2");

		return { val1, val2 };
	}

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			// result = ConvertSet(pm.GetIfStmtVarPairWithSubPattern("", ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			// result = ConvertSet(pm.GetWhileStmtVarPairWithSubPattern("", ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithSubPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			// result = ConvertSet(pm.GetIfStmtVarPairWithFullPattern("", ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			// result = ConvertSet(pm.GetWhileStmtVarPairWithFullPattern("", ParsingEntRef(param2)));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithFullPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateOneSynonymSet(STRING f_call, STRING param) {
	PKB pkb;
	RelationManager rm = pkb.GetRelationManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateOneSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(rm.GetFollows(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(rm.GetFollowsStars(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(rm.GetParents(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(rm.GetParentStars(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm.GetStmtUses(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm.GetProcUses(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm.GetStmtModifies(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm.GetProcModifies(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		// result = rm.GetCalls(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		// result = rm.GetCallsStar(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		// result = rm.GetNext(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		// result = rm.GetNextStar(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		// result = rm.GetAffects(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		// result = rm.GetAffectsStar(ParsingStmtRef(param));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateOneSynonymSet: No such relref." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateOneSynonymSet: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateInverseOneSynonymSet(STRING f_call, STRING param) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(rm.GetInverseFollows(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(rm.GetInverseFollowsStars(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(rm.GetInverseParents(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(rm.GetInverseParentStars(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm.GetInverseStmtUses(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm.GetInverseProcUses(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm.GetInverseStmtModifies(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm.GetInverseProcModifies(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		// result = rm.GetInverseCalls(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		// result = rm.GetInverseCallsStar(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		// result = rm.GetInverseNext(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		// result = rm.GetInverseNextStar(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		// result = rm.GetInverseAffects(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		// result = rm.GetInverseAffectsStar(ParsingStmtRef(param));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateInverseOneSynonymSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneSynonymSet: Result clause's size = " << result.size() << endl;
	}

	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateTwoSynonymSet(STRING f_call) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateTwoSynonymSet" << endl;
		cout << "fcall: " << f_call << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateTwoSynonymSet - UNIT TESTING" << endl;
		if (f_call.compare(TYPE_COND_USES_S) == 0 || f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
			STRING_LIST* val1 = new STRING_LIST();
			val1->push_back("1");
			val1->push_back("v1");
			STRING_LIST* val2 = new STRING_LIST();
			val2->push_back("2");
			val2->push_back("v2");

			return { val1, val2 };

		}
		else if (f_call.compare(TYPE_COND_USES_P) == 0 || f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
			STRING_LIST* val1 = new STRING_LIST();
			val1->push_back("p1");
			val1->push_back("v2");
			STRING_LIST* val2 = new STRING_LIST();
			val2->push_back("p2");
			val2->push_back("v3");

			return { val1, val2 };
		}
		else {
			STRING_LIST* val1 = new STRING_LIST();
			val1->push_back("1");
			val1->push_back("2");
			STRING_LIST* val2 = new STRING_LIST();
			val2->push_back("2");
			val2->push_back("3");

			return { val1, val2 };
		}
	}

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(rm.GetAllFollows());
	}
	else if (f_call.compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(rm.GetAllFollowsStars());
	}
	else if (f_call.compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(rm.GetAllParents());
	}
	else if (f_call.compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(rm.GetAllParentStars());
	}
	else if (f_call.compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm.GetAllStmtUses());
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm.GetAllProcUses());
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm.GetAllStmtModifies());
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm.GetAllProcModifies());
	}
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		// result = ConvertSet(rm.GetAllCalls());
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		// result = ConvertSet(rm.GetAllCallsStar());
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		// result = ConvertSet(rm.GetAllNext());
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		// result = ConvertSet(rm.GetAllNextStar());
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		// result = ConvertSet(rm.GetAllAffects());
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		// result = ConvertSet(rm.GetAllAffectsStar());
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateTwoSynonymSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateTwoSynonymSet: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateAllCall(STRING output_var_type) {
	PKB pkb = PKB();
	DataManager dm = pkb.GetDataManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAllCall" << endl;
		cout << "output_var_type: " << output_var_type << endl;
	}

	if (output_var_type.compare(TYPE_STMT) == 0 || output_var_type.compare(TYPE_PROG_LINE) == 0) {
		result = ConvertSet(dm.GetAllStatements());
	}
	else if (output_var_type.compare(TYPE_STMT_ASSIGN) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::assignStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_CALL) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::callStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_IF) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::ifStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_WHILE) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::whileStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_PRINT) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::printStatement));
	}
	else if (output_var_type.compare(TYPE_STMT_READ) == 0) {
		result = ConvertSet(dm.GetAllStatements(STATEMENT_TYPE::readStatement));
	}
	else if (output_var_type.compare(TYPE_VAR) == 0) {
		result = ConvertSet(dm.GetAllVariables());
	}
	else if (output_var_type.compare(TYPE_PROC) == 0) {
		result = ConvertSet(dm.GetAllProcedures());
	}
	else if (output_var_type.compare(TYPE_CONST) == 0) {
		result = ConvertSet(dm.GetAllConstants());
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateAllCall: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAllCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::ConvertSet(STRING_SET result_set) {
	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STRING k : result_set) {
			STRING_LIST* value = new STRING_LIST();
			value->push_back(k);

			final_result.insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET PQLEvaluator::ConvertSet(INTEGER_SET result_set) {
	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (INTEGER k : result_set) {
			STRING_LIST* value = new STRING_LIST();
			value->push_back(to_string(k));

			final_result.insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET PQLEvaluator::ConvertSet(DOUBLE_SET result_set) {
	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (DOUBLE k : result_set) {
			char convert_to_str[50];
			sprintf(convert_to_str, "%.0f", k);

			STRING_LIST* value = new STRING_LIST();
			value->push_back(convert_to_str);

			final_result.insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET PQLEvaluator::ConvertSet(STMT_STMT_PAIR_LIST result_set) {
	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STMT_STMT_PAIR k : result_set) {
			STRING first = to_string(k.s1);
			STRING second = to_string(k.s2);

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLEvaluator::ConvertSet(STMT_VAR_PAIR_LIST result_set) {
	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STMT_VAR_PAIR k : result_set) {
			STRING first = to_string(k.s);
			STRING second = k.v;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLEvaluator::ConvertSet(PROC_VAR_PAIR_LIST result_set) {
	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (PROC_VAR_PAIR k : result_set) {
			STRING first = k.p;
			STRING second = k.v;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

BOOLEAN PQLEvaluator::CheckConstraints(STRINGPAIR_SET constraints, STRING_STRING_MAP entity_map, STRING_LIST key, STRINGLIST_SET* value) {
	for (STRING_PAIR* check : constraints) {
		STRING lhs = check->first;
		STRING rhs = check->second;
		/*
		for (int key_index = 0; key_index < key.size(); key_index++) {
			STRING_LIST* k = new STRING_LIST();

			if (lhs.compare(key[key_index]) == 0) {
				// Check for rhs' result in results_map
				k = GetRelatedSynonyms(rhs, results_map);
			}
			else if (rhs.compare(key[key_index]) == 0) {
				// Check for lhs' result in results_map
				k = GetRelatedSynonyms(lhs, results_map);
			}
			else {
				k = nullptr;
			}

			if (k != nullptr) {
				INTEGER index = distance(k->begin(), find(k->begin(), k->end(), lhs));
				STRING_SET v = GetNewResult(results_map.at(k), index);
				value = new STRINGLIST_SET(GetIntersectResult(v, *value, key_index));
			}
		}
		*/
	}

	return true;
}
/*
VOID PQLEvaluator::CheckConstraints(STRINGPAIR_SET constraints, STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST key, STRING_SET* value) {
	for (STRING_PAIR* check : constraints) {
		STRING lhs = check->first;
		STRING rhs = check->second;

		for (STRING check_key : key) {
			STRING_LIST* k = new STRING_LIST();

			if (lhs.compare(check_key) == 0) {
				// Check for rhs' result in results_map
				k = GetRelatedSynonyms(rhs, results_map);
			}
			else if (rhs.compare(check_key) == 0) {
				// Check for lhs' result in results_map
				k = GetRelatedSynonyms(lhs, results_map);
			}
			else {
				k = nullptr;
			}

			if (k != nullptr) {
				INTEGER index = distance(k->begin(), find(k->begin(), k->end(), lhs));
				STRING_SET v = GetNewResult(results_map.at(k), index);
				value = new STRING_SET(GetIntersectResult(*value, v));
			}
		}
	}
}
*/
BOOLEAN PQLEvaluator::RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, INTEGER pos_to_check) {
	BOOLEAN is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		STRING v = (*it)->at(pos_to_check);
		BOOLEAN has_value = false;
		for (STRING_LIST* tmp_entry : tmp) {
			STRING check = tmp_entry->at(0);
			if (check == v) {
				has_value = true;
			}
		}

		if (has_value) {
			it++;
		}
		else {
			it = value->erase(it);
			is_changed = true;
		}
	}

	return is_changed;
}

STRINGLIST_SET PQLEvaluator::GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, INTEGER pos_to_check) {
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	for (STRING_LIST* i : val2) {
		for (STRING j : val1) {
			if ((*i).at(pos_to_check) == j) {
				result.insert(i);
			}
		}
	}

	return result;
}

STRING_SET PQLEvaluator::GetIntersectResult(STRING_SET val1, STRING_SET val2) {
	STRING_SET result = *(new STRING_SET());

	for (auto i = val1.begin(); i != val1.end(); i++) {
		if (val2.find(*i) != val2.end()) {
			result.insert(*i);
		}
	}

	return result;
}

STRING_SET PQLEvaluator::GetNewResult(STRINGLIST_SET value, INTEGER pos_to_check) {
	if (DEBUG) {
		cout << "PQLEvaluator - GetNewResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : value) {
		result.insert((*i).at(pos_to_check));
	}

	return result;
}

STRINGLIST_SET PQLEvaluator::GetNewResult(STRINGLIST_SET value, INTEGER_SET pos_to_check) {
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	for (STRING_LIST* entry : value) {
		STRING_LIST* entry_value = new STRING_LIST();

		for (INTEGER i = 0; i < entry->size(); i++) {
			if (pos_to_check.find(i) != pos_to_check.end()) {
				// Found value to be included in the result
				entry_value->push_back(entry->at(i));
			}
		}

		result.insert(entry_value);
	}

	return result;
}

INTEGERLIST_LIST PQLEvaluator::GetCommonSynonymsIndex(STRING_LIST large_keys, STRING_LIST small_keys) {
	INTEGERLIST_LIST results = *(new INTEGERLIST_LIST());

	for (INTEGER s_index = 0; s_index < small_keys.size(); s_index++) {
		for (INTEGER l_index = 0; l_index < large_keys.size(); l_index++) {
			if (small_keys[s_index].compare(large_keys[l_index]) == 0) {
				INTEGER_LIST common_index = *(new INTEGER_LIST());
				// l_key = s_key
				common_index.push_back(l_index);
				common_index.push_back(s_index);
				results.push_back(common_index);

				// should not have duplicated key
				break;
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLEvaluator::GetCombinedResult(STRINGLIST_SET large_values, STRINGLIST_SET small_values, INTEGERLIST_LIST indexes) {
	STRINGLIST_SET results = *(new STRINGLIST_SET());

	for (STRING_LIST* l_entry : large_values) {
		for (STRING_LIST* s_entry : small_values) {
			BOOLEAN is_common = true;
			INTEGER_SET index_to_exclude = INTEGER_SET();

			for (INTEGER_LIST index_entry : indexes) {
				if (l_entry[index_entry[0]] != s_entry[index_entry[1]]) {
					is_common = false;
					break;
				}
				else {
					index_to_exclude.insert(index_entry[1]);
				}
			}

			if (is_common) {
				STRING_LIST* entry = new STRING_LIST();
				entry = l_entry;
				for (INTEGER s = 0; s < s_entry->size(); s++) {
					if (index_to_exclude.find(s) == index_to_exclude.end()) {
						entry->push_back(s_entry->at(s));
					}
				}

				results.insert(entry);
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLEvaluator::GetCombinedResult(STRINGLIST_SET results, STRINGLIST_SET values) {
	if (results.empty()) {
		results = values;
	}
	else {
		for (STRING_LIST* value : results) {
			for (auto v = values.cbegin(); v != values.cend(); v++) {
				value->push_back((*v)->at(0));
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLEvaluator::GetCombinedResult(STRINGLIST_SET results, STRINGLIST_SET values, INTEGER pos_to_add, INTEGERPAIR_SET to_check) {
	if (results.empty()) {
		results = values;
	}
	else {
		for (auto r = results.cbegin(); r != results.cend(); r++) {
			for (auto v = values.cbegin(); v != values.cend(); v++) {
				for (INTEGER_PAIR* pair : to_check) {
					INTEGER r_index = pair->first;
					INTEGER v_index = pair->second;

					if ((*r)->at(r_index) == (*v)->at(v_index)) {
						(*r)->push_back((*v)->at(pos_to_add));
					}
				}
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLEvaluator::GetCombinedResult(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list) {
	STRINGLIST_SET results = *(new STRINGLIST_SET());

	STRING_LIST added_output = STRING_LIST();
	for (STRING output : output_list) {
		for (auto entry = results_map.cbegin(); entry != results_map.cend(); entry++) {
			STRING_LIST* synonyms = (*entry).first;
			STRINGLIST_SET values = (*entry).second;
			BOOLEAN is_tuple = synonyms->size() > 1 ? true : false;

			for (INTEGER i = 0; i < synonyms->size(); i++) {
				
				if (synonyms->at(i).compare(output) == 0) {
					// output synonym found
					if (!is_tuple) {
						results = GetCombinedResult(results, values);
					}
					else {
						// check constraint
						INTEGERPAIR_SET added_index = INTEGERPAIR_SET();
						for (INTEGER s = 0; s < synonyms->size(); s++) {
							for (INTEGER a = 0; a < added_output.size(); a++) {
								if (synonyms->at(s).compare(added_output.at(a)) == 0) {
									INTEGER_PAIR* pair = new INTEGER_PAIR();
									pair->first = a;
									pair->second = s;

									added_index.insert(pair);
								}
							}
						}

						if (added_index.empty()) {
							results = GetCombinedResult(results, values);
						}
						else {
							results = GetCombinedResult(results, values, i, added_index);
						}
					}

					added_output.push_back(output);
				}
			}
		}
	}

	return results;
}

BOOLEAN PQLEvaluator::IsVar(STRING var) {
	if (var[0] == '\"' && var[var.length() - 1] == '\"') {
		return false;
	}
	else {
		return true;
	}

	;
}

BOOLEAN PQLEvaluator::IsBooleanOutput(STRING_LIST output_list) {
	if (output_list.size() == 1 && output_list[0] == "BOOLEAN") {
		return true;
	}
	else {
		return false;
	}
}

BOOLEAN PQLEvaluator::IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list) {
	for (STRING synonym : synonyms) {
		for (STRING output : output_list) {
			if (synonym.compare(output) == 0) {
				return true;
			}
		}
	}

	return false;
}

INTEGER PQLEvaluator::ParsingStmtRef(STRING param) {
	if (param.compare("_") == 0 || param.compare("") == 0) {
		return -1;
	}
	else {
		return stoi(param);
	}
}

STRING PQLEvaluator::ParsingEntRef(STRING param) {
	if (param.compare("_") == 0) {
		return "";
	}
	else {
		// trim away the quotation marks
		param.erase(remove(param.begin(), param.end(), '\"'), param.end());
		return param;
	}
}