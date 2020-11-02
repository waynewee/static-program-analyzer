#include "PQLOptimizedEvaluator.h"
#include <iostream>

#include "pkb/PKB.h"

QueryResult PQLOptimizedEvaluator::Evaluate(QueryInfo query_info) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - Evaluate" << endl;
	}

	// Final result to be returned
	STRINGLIST_SET final_result_set = *(new STRINGLIST_SET());

	// Expected output list
	STRING_LIST output_list = query_info.GetOutputList();
	bool is_boolean_output = query_info.IsBooleanOutput();

	// Unevaluated clauses & constraints
	STRINGSET_FUNCTIONQUEUE_MAP synonyms_map = STRINGSET_FUNCTIONQUEUE_MAP();
	CONSTRAINT_QUEUE constraints = CONSTRAINT_QUEUE();
	STRING_SET constraint_synonyms = STRING_SET();

	// Occurrence count - optimization
	STRING_INTEGER_MAP constraint_occurrence_count = STRING_INTEGER_MAP();
	STRING_INTEGER_MAP clause_occurrence_count = STRING_INTEGER_MAP();
	
	// Intermediate results from evaluated clauses
	STRINGLIST_STRINGLISTSET_MAP results_map = STRINGLIST_STRINGLISTSET_MAP();
	STRINGLIST_STRINGLISTSET_MAP final_results_map = STRINGLIST_STRINGLISTSET_MAP();

	// Query Info object
	STRING_STRING_MAP entity_map = query_info.GetEntityMap();

	// Parse clauses conditions
	bool has_parsed = true;
	
	STRINGPAIR_SET tmp_constraints = STRINGPAIR_SET();
	if (!ParseClauses(query_info, &tmp_constraints, &constraint_occurrence_count, &constraint_synonyms)) {
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - parsing with clause: failed." << endl;
		}

		has_parsed = false;
	}

	STRINGSET_STRINGLISTSET_MAP tmp_map = STRINGSET_STRINGLISTSET_MAP();
	if (!ParseClauses(query_info, &tmp_map, &clause_occurrence_count)) {
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - parsing such that and pattern clause: failed." << endl;
		}

		has_parsed = false;
	}

	if (!has_parsed) {
		return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
	}

	if (DEBUG_PRINTING) {
		cout << "constraints after parsing with clause" << endl;
		Print(tmp_constraints);
		cout << "synonym map after parsing st and pattern clause" << endl;
		Print(tmp_map);
	}

	// Set priority for constraints
	constraints = CONSTRAINT_QUEUE(constraint_occurrence_count);
	for (STRING_PAIR* constraint_pair : tmp_constraints) {
		constraints.push(*constraint_pair);
	}

	// Set priority for clauses
	for (auto f = tmp_map.cbegin(); f != tmp_map.cend(); f++) {
		FUNCTION_QUEUE priority = FUNCTION_QUEUE(clause_occurrence_count);
		STRINGLIST_SET clauses = (*f).second;

		for (auto c = clauses.cbegin(); c != clauses.cend(); c++) {
			priority.push(**c);
		}

		synonyms_map.insert({ new STRING_SET(*(*f).first), priority });
	}

	if (DEBUG_PRINTING) {
		cout << "constraints aft setting priority" << endl;
		Print(constraints);
		cout << "synonym map aft setting priority" << endl;
		Print(synonyms_map);
	}

	// Evaluate constraints
	if (!EvaluateConstraints(entity_map, constraints, &results_map)) {
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - Evaluating with clauses: Constraint not met." << endl;
		}

		return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
	}

	if (DEBUG_PRINTING) {
		cout << "Results map after evaluating constraints" << endl;
		Print(results_map);
	}
	
	// Evaluate synonyms_map: 1 related-synonyms group at a time
	for (auto f = synonyms_map.cbegin(); f != synonyms_map.cend(); f++) {
		STRINGLIST_STRINGLISTSET_MAP tmp_map = results_map;
		FUNCTION_QUEUE clauses = (*f).second;
		while (!clauses.empty()) {
			STRING_LIST c = clauses.top(); 
			clauses.pop();

			string f_call = c.at(0);
			string param1 = c.at(1);
			string param2 = c.at(2);

			STRING_LIST c_key = STRING_LIST();
			STRINGLIST_SET c_value = STRINGLIST_SET();
			
			if (f_call.compare(TYPE_COND_PATTERN_F) == 0 || f_call.compare(TYPE_COND_PATTERN_P) == 0) {
				string param3 = c.at(3);
				if (c.size() == 5) {
					param3 = c.at(4);
				}
				c_key.push_back(param3);

				if (IsVar(param1)) {
					// first param = synonym
					c_key.push_back(param1);
					c_value = EvaluatePatternCall(f_call, param2, entity_map.at(param3));
				}
				else {
					// first param != synonym
					c_value = EvaluatePatternCall(f_call, param1, param2, entity_map.at(param3));
				}
			}
			else {
				if (IsVar(param1) && IsVar(param2)) {
					// both params = synonyms
					c_key.push_back(param1);
					c_key.push_back(param2);
					c_value = EvaluateTwoSynonymSet(f_call);

					STRINGLIST_SET tmp = EvaluateAllCall(entity_map.at(param1));
					RemoveIrrelevant(&c_value, tmp, 0);

					tmp = EvaluateAllCall(entity_map.at(param2));
					RemoveIrrelevant(&c_value, tmp, 1);

					if (param1.compare(param2) == 0) {
						// same synonym
						tmp.clear();
						for (STRING_LIST* s : c_value) {
							if (s->at(0).compare(s->at(1)) == 0) {
								STRING_LIST* to_insert = new STRING_LIST();
								to_insert->push_back(s->at(0));
								tmp.insert(to_insert);
							}
						}
						c_value = tmp;
					}
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
					cout << "PQLOptimizedEvaluator - Evaluating synonyms_map: Empty results." << endl;
				}

				return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
			}

			if (DEBUG_PRINTING) {
				cout << "Results after evaluating 1 clause" << endl;
				cout << "KEY " << endl;
				Print(c_key);
				cout << "VALUE " << endl;
				Print(c_value);
			}

			// Add into the tmp_map
			AddResult(c_key, c_value, &tmp_map);
			if (tmp_map.empty()) {
				return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
			}
		}

		if (DEBUG_PRINTING) {
			cout << "Results after evaluating 1 group of clauses" << endl;
			Print(tmp_map);
		}

		// Check if any of the synonyms is in expected output list: "TRUE" -> set tmp_map as results_map; "FALSE" -> continue
		STRING_SET synonyms = *(*f).first;
		if (IsOutputSynonyms(synonyms, output_list) || IsConstraintSynonym(synonyms, constraint_synonyms)) {
			results_map = tmp_map;
		}

		if (DEBUG_PRINTING) {
			cout << "Results map after inserting 1 group's results" << endl;
			Print(results_map);
		}
	}

	if (DEBUG_PRINTING) {
		cout << "Results map after evaluating clauses" << endl;
		Print(results_map);
	}

	if (!is_boolean_output) {	
		// Get output_list's results from results_map
		INTEGER_SET retrieved_output_index = INTEGER_SET();

		for (int o = 0; o < output_list.size(); o++) {
			string parsed_output_synonym = ParsingSynonym(output_list.at(o));
			string output_synonym_attr = ParsingSynonymAttribute(output_list.at(o));

			for (auto result_entry = results_map.cbegin(); result_entry != results_map.cend(); result_entry++) {
				STRING_LIST* result_key = (*result_entry).first;
				bool is_found = false;
				for (int i = 0; i < result_key->size(); i++) {
					if (result_key->at(i).compare(parsed_output_synonym) == 0) {
						// found output synonym
						is_found = true;

						// Add index of retrieved output synonym to list
						retrieved_output_index.insert(o);
					}
				}

				if (is_found) {
					// Has >=1 expected output synonym(s)
					AddResult(*result_key, (*result_entry).second, &final_results_map);
					if (final_results_map.empty()) {
						return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
					}
				}
			}
		}

		if (DEBUG_PRINTING) {
			cout << "Final results map after extracting dependent output synonyms " << endl;
			Print(final_results_map);
		}

		// Get all output_list's results that are not in results_map
		for (int i = 0; i < output_list.size(); i++) {
			if (retrieved_output_index.find(i) == retrieved_output_index.end()) {
				string synonym = output_list.at(i);
				string parsed_synonym = ParsingSynonym(synonym);
				string synonym_type = ParsingSynonymAttribute(synonym);

				STRING_LIST key = *(new STRING_LIST());
				key.push_back(parsed_synonym);
				STRINGLIST_SET value = *(new STRINGLIST_SET());

				STRING_LIST* check_key = GetRelatedSynonyms(key, final_results_map);
				if (check_key == nullptr) {
					// not duplicate
					value = EvaluateAllCall(entity_map.at(parsed_synonym));

					if (value.empty()) {
						// error
						if (DEBUG) {
							cout << "PQLOptimizedEvaluator - Extracting independent output synonyms: Empty results." << endl;
						}

						return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
					}
				}
				else {
					// duplicate
					STRING_LIST new_key = STRING_LIST();
					new_key.insert(new_key.end(), check_key->begin(), check_key->end());
					new_key.insert(new_key.end(), key.begin(), key.end());
					key = new_key;

					value = final_results_map.at(check_key);
					for (auto v = value.cbegin(); v != value.cend(); v++) {
						(*v)->push_back((*v)->at(0));
					}

					final_results_map.erase(check_key);
				}
				final_results_map.insert({ new STRING_LIST(key), value });
			}
		}
	
		if (DEBUG_PRINTING) {
			cout << "Final results map after extracting independent output synonyms " << endl;
			Print(final_results_map);
		}

		// Combine all the expected output results
		final_result_set = GetCartesianProduct(final_results_map, output_list, entity_map);
	}

	if (DEBUG_PRINTING) {
		cout << "final result set" << endl;
		Print(final_result_set);
	}

	return SetResult(is_boolean_output, "TRUE", final_result_set);
}

void PQLOptimizedEvaluator::Print(STRINGPAIR_SET to_print) {
	for (STRING_PAIR* p : to_print) {
		cout << "<" << p->first << "=" << p->second << "> ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(INTEGERPAIR_SET to_print) {
	for (INTEGER_PAIR* p : to_print) {
		cout << "<" << p->first << "=" << p->second << "> ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(STRINGLIST_STRINGLISTSET_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		for (string s : *(*result_entry).first) {
			cout << s << " ";
		}
		cout << ": ";;
		Print((*result_entry).second);
	}
}

void PQLOptimizedEvaluator::Print(STRINGSET_STRINGLISTSET_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		Print(*(*result_entry).first);
		cout << ": ";
		Print((*result_entry).second);
	}
}

void PQLOptimizedEvaluator::Print(STRINGSET_FUNCTIONQUEUE_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		Print(*(*result_entry).first);
		cout << ": ";
		Print((*result_entry).second);
	}
}

void PQLOptimizedEvaluator::Print(FUNCTION_QUEUE to_print) {
	while(!to_print.empty()) {
		STRING_LIST values = to_print.top();
		to_print.pop();
		Print(values);
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(CONSTRAINT_QUEUE to_print) {
	while (!to_print.empty()) {
		STRING_PAIR values = to_print.top();
		to_print.pop();
		cout << "<" << values.first << "=" << values.second << "> ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(STRING_SET to_print) {
	for (auto f = to_print.cbegin(); f != to_print.cend(); f++) {
		cout << (*f) << " ";
	}
	cout << endl;
}

void PQLOptimizedEvaluator::Print(STRING_LIST to_print) {
	cout << "{ ";
	for (string s : to_print) {
		cout << s << " ";
	}
	cout << "} ";
}

void PQLOptimizedEvaluator::Print(STRINGLIST_SET to_print) {
	for (STRING_LIST* set : to_print) {
		Print(*set);
	}
	cout << endl;
}

void PQLOptimizedEvaluator::AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP* results_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - AddResult" << endl;
	}

	STRING_LIST* check_key = GetRelatedSynonyms(key, *results_map);
	if (check_key == nullptr) {
		// not related (yet)
		results_map->insert({ new STRING_LIST(key), value });
	}
	else {
		STRINGLIST_SET tmp_result = results_map->at(check_key);
		STRING_LIST new_key = *(new STRING_LIST(*check_key));
		bool is_empty = false;

		while (check_key != nullptr) {
			// Merge common synonyms & their results
			INTEGERPAIR_SET index_to_compare = INTEGERPAIR_SET();
			INTEGER_SET pos_to_ignore = INTEGER_SET();

			for (int s = 0; s < check_key->size(); s++) {
				for (int a = 0; a < key.size(); a++) {
					if (check_key->at(s).compare(key.at(a)) == 0) {
						// common synonym
						INTEGER_PAIR* pair = new INTEGER_PAIR();
						pair->first = s;
						pair->second = a;

						index_to_compare.insert(pair);
						pos_to_ignore.insert(a);
					}
				}
			}
			cout << "key: ";
			Print(new_key);
			cout << "tmp result: ";
			Print(tmp_result);
			cout << "key: ";
			Print(*check_key);
			cout << "value: ";
			Print(value);
			cout << endl;
			if (pos_to_ignore.size() == key.size()) {
				tmp_result = GetDependencyProduct(tmp_result, value, -1, index_to_compare, "", "");
			}
			else {
				for (int i = 0; i < key.size(); i++) {
					if (pos_to_ignore.find(i) == pos_to_ignore.end()) {
						new_key.push_back(key.at(i));

						cout << "pos to be added: " << i << endl;
						cout << "index to compare ";
						Print(index_to_compare);
						tmp_result = GetDependencyProduct(tmp_result, value, i, index_to_compare, "", "");
					}
				}
			}

			if (tmp_result.empty()) {
				if (DEBUG) {
					cout << "PQLOptimizedEvaluator: AddResult: Empty result." << endl;
				}

				results_map->clear();
				is_empty = true;
				break;
			}
			else {
				results_map->erase(check_key);				
			}

			// Stop condition: no more new synonyms
			check_key = GetRelatedSynonyms(new_key, *results_map);
			if (check_key != nullptr) {
				Print(*check_key);

			}
			else {
				cout << "check key = null ptr" << endl;
			}
		}

		if (!is_empty) {
			results_map->insert({ new STRING_LIST(new_key), tmp_result });
			Print(*results_map);
		}
	} 
}

QueryResult PQLOptimizedEvaluator::SetResult(bool is_boolean_output, string bool_result, STRINGLIST_SET result) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - SetResult" << endl;
	}

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

bool PQLOptimizedEvaluator::ParseClauses(QueryInfo query_info, STRINGPAIR_SET* constraints, STRING_INTEGER_MAP* occurrence_count, STRING_SET* constraint_synonyms) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ParseClauses: WITH" << endl;
	}

	STRINGPAIR_SET with_map = query_info.GetWithMap();
	for (STRING_PAIR* clause: with_map) {
		string lhs = clause->first;
		string rhs = clause->second;

		if (DEBUG) {
			cout << "LHS: " << lhs << ", RHS: " << rhs << endl;
		}

		if (!IsVar(lhs) && !IsVar(rhs)) {
			// INT=INT or STRING=STRING
			// Check if it is equal
			// If "FALSE" -> return false, else continue
			if (lhs.compare(rhs) != 0) {
				return false;
			}
		}
		else {
			if (!IsVar(lhs) && IsVar(rhs)) {
				// INT/STRING=synonyms
				clause->first = rhs;
				clause->second = lhs;
			}
			
			if (!(IsVar(rhs) && lhs.compare(rhs) == 0)) {
				// synonym=synonym: different synonyms
				// or synonym=INT or synonym=STRING
				if (constraints->insert(clause).second == 0) {
					// error
					if (DEBUG) {
						cout << "PQLOptimizedEvaluator - Parsing clauses: Error inserting constraints." << endl;
					}
					return false;
				}

				// Increase occurrence count & add synonym into constraint_synonyms
				IncreaseOccurrenceCount(ParsingSynonym(lhs), occurrence_count);
				constraint_synonyms->insert(ParsingSynonym(lhs));
				if (IsVar(rhs)) {
					IncreaseOccurrenceCount(ParsingSynonym(rhs), occurrence_count);
					constraint_synonyms->insert(ParsingSynonym(rhs));
				}
			}
		}
		
	}

	return true;
}

bool PQLOptimizedEvaluator::ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map, STRING_INTEGER_MAP* occurrence_count) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ParseClauses: Such that and Pattern" << endl;
	}

	STRING_STRINGLISTLIST_MAP st_map = query_info.GetStMap();
	STRING_STRINGLISTLIST_MAP pattern_map = query_info.GetPatternMap();

	for (auto f = st_map.cbegin(); f != st_map.cend(); f++) {
		string f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			if (p[0].compare(p[1]) == 0 && !IsUnderscore(p[0]) && f_call.compare(TYPE_COND_NEXT_T) != 0) {
				// same parameter & not _ & not Next*
				if (DEBUG) {
					cout << "PQLOptimizedEvaluator - Evaluating such that clauses: Same parameters." << endl;
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
					cout << "PQLOptimizedEvaluator - Parsing such that clauses: Error creating value for such that clauses." << endl;
				}

				return false;
			}

			if (!IsVar(p[0]) && !IsVar(p[1])) {
				// both params != synonyms
				string param1 = IsVar(clause->at(1)) ? "" : clause->at(1);
				string param2 = IsVar(clause->at(2)) ? "" : clause->at(2);

				if (!EvaluateNoSynonymSet(f_call, param1, param2)) {
					if (DEBUG) {
						cout << "PQLOptimizedEvaluator - Evaluating no synonym clauses: False clause." << endl;
					}

					return false;
				}
			}
			else {
				STRING_SET* key = new STRING_SET();
				STRING_SET* check_key = new STRING_SET();
				STRINGLIST_SET value = *(new STRINGLIST_SET());

				if (IsVar(p[0]) && !IsVar(p[1])) {
					// 1st param == synonym
					key->insert(p[0]);
					check_key = GetRelatedSynonyms(p[0], *synonyms_map);

					// Increase count for synonym
					IncreaseOccurrenceCount(p[0], occurrence_count);
				}
				else if (!IsVar(p[0]) && IsVar(p[1])) {
					// 2nd param == synonym
					key->insert(p[1]);
					check_key = GetRelatedSynonyms(p[1], *synonyms_map);

					// Increase count for synonym
					IncreaseOccurrenceCount(p[1], occurrence_count);
				}
				else if (IsVar(p[0]) && IsVar(p[1])) {
					// 2 params = synonyms
					key->insert(p[0]);
					key->insert(p[1]);
					check_key = GetRelatedSynonyms(*key, synonyms_map);

					// Increase count for synonym
					IncreaseOccurrenceCount(p[0], occurrence_count);
					IncreaseOccurrenceCount(p[1], occurrence_count);
				}
				else {
					// error
					if (DEBUG) {
						cout << "PQLOptimizedEvaluator - Parsing such that clauses: Invalid parameters." << endl;
					}

					return false;
				}

				if (check_key != nullptr) {
					key = check_key;
					value = synonyms_map->at(key);
					value.insert(clause);
					synonyms_map->at(key) = value;
				}
				else {
					value.insert(clause);
					if (synonyms_map->insert({ key, value }).second == 0) {
						// error
						if (DEBUG) {
							cout << "PQLOptimizedEvaluator - Parsing such that clauses: Error inserting into synonyms_map." << endl;
						}
						return false;
					}
				}
			}
		}
	}

	for (auto f = pattern_map.cbegin(); f != pattern_map.cend(); f++) {
		string f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			STRING_LIST* clause = new STRING_LIST();
			clause->push_back(f_call);
			clause->push_back(p[0]);
			clause->push_back(p[1]);
			clause->push_back(p[2]);

			string type = p[2];
			if (p.size() == 4) {
				// If pattern
				clause->push_back(p[3]);
				type = p[3];
			}

			if (clause->empty()) {
				// error
				if (DEBUG) {
					cout << "PQLOptimizedEvaluator - Parsing clauses: Error creating value for pattern clauses." << endl;
				}

				return false;
			}

			STRING_SET* key = new STRING_SET();
			key->insert(type);

			// Increase count for synonym
			IncreaseOccurrenceCount(type, occurrence_count);

			STRING_SET* check_key = new STRING_SET();
			STRINGLIST_SET value = *(new STRINGLIST_SET());

			if (IsVar(p[0])) {
				// 1st param = synonym 
				key->insert(p[0]);

				// Increase count for synonym
				IncreaseOccurrenceCount(p[0], occurrence_count);
			}

			check_key = GetRelatedSynonyms(*key, synonyms_map);

			if (check_key != nullptr) {
				key = check_key;
				value = synonyms_map->at(key);
				value.insert(clause);
				synonyms_map->at(key) = value;
			}
			else {
				value.insert(clause);
				if (synonyms_map->insert({ key, value }).second == 0) {
					// error
					if (DEBUG) {
						cout << "PQLOptimizedEvaluator - Parsing pattern clauses: Error inserting into synonyms_map." << endl;
					}
					return false;
				}
			}
		}
	}

	return true;
}

STRING_LIST* PQLOptimizedEvaluator::GetRelatedSynonyms(string synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		for (string s: *existing_key) {
			// found related synonyms in the synonyms_map
			if (s.compare(synonym) == 0) {
				return existing_key;
			}
		}
	}

	return nullptr;
}

STRING_LIST* PQLOptimizedEvaluator::GetRelatedSynonyms(STRING_LIST synonyms, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}
	
	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		for (string existing_synonym: *existing_key) {
			// found related synonyms in the synonyms_map
			for (string checking_synonym: synonyms) {
				if (existing_synonym.compare(checking_synonym) == 0) {
					return existing_key;
				}
			}
		}
	}

	return nullptr;
}

STRING_SET* PQLOptimizedEvaluator::GetRelatedSynonyms(string synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_SET* existing_key = (*k).first;
		if (existing_key->find(synonym) != existing_key->end()) {
			// found related synonyms in the synonyms_map
			return existing_key;
		}
	}

	return nullptr;
}

STRING_SET* PQLOptimizedEvaluator::GetRelatedSynonyms(STRING_SET synonyms, STRINGSET_STRINGLISTSET_MAP* synonyms_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetRelatedSynonyms" << endl;
	}

	bool has_related_synonyms = false;

	STRING_SET* new_keys = new STRING_SET();
	STRINGLIST_SET new_values = *(new STRINGLIST_SET());

	for (auto k = synonyms.cbegin(); k != synonyms.cend(); k++) {
		Print(*synonyms_map);
		string curr_check = *k;
		STRING_SET* check_key = GetRelatedSynonyms(curr_check, *synonyms_map);
		while (check_key != nullptr) {
			// found related synonyms in the synonyms_map
			new_keys->insert(check_key->begin(), check_key->end());

			// combine the clauses
			new_values.insert(synonyms_map->at(check_key).begin(), synonyms_map->at(check_key).end());

			// remove the existing synonyms since already combined
			synonyms_map->erase(check_key);
			check_key = GetRelatedSynonyms(curr_check, *synonyms_map);
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

bool PQLOptimizedEvaluator::EvaluateNoSynonymSet(string f_call, string param1, string param2) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateNoSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param1: " << param1 << "; param2: " << param2 << endl;
	}

	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	CFGManager cfgm = pkb.GetCFGManager();

	if (UNIT_TESTING) {
		cout << "PQLOptimizedEvaluator - EvaluateNoSynonymSet - UNIT TESTING" << endl;
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
	else if (f_call.compare(TYPE_COND_CALLS) == 0) {
		return rm.IsCalls(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		return rm.IsCallsStar(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		return cfgm.IsNext(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		return cfgm.IsNextStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		return am.IsAffects(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		return am.IsAffectsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateNoSynonymSet: No such relref." << endl;
		}

		return false;
	}
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluatePatternCall(string f_call, string param1, string param2, string type) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall (2 params to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param1 << "; param2: " << param2 << "; type: " << type << endl;
	}
	
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			result = ConvertSet(pm.GetIfWithPattern(ParsingEntRef(param1)));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			result = ConvertSet(pm.GetWhileWithPattern(ParsingEntRef(param1)));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithFullPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluatePatternCall(string f_call, string param, string type) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall (1 param to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param << "; type: " << type << endl;
	}

	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithSubPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			result = ConvertSet(pm.GetIfStmtVarPairWithPattern(""));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			result = ConvertSet(pm.GetWhileStmtVarPairWithPattern(""));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithFullPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (DEBUG) {
				cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluatePatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluatePatternCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

bool PQLOptimizedEvaluator::EvaluateConstraints(STRING_STRING_MAP entity_map, CONSTRAINT_QUEUE constraints, STRINGLIST_STRINGLISTSET_MAP* results_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateConstraints" << endl;
	}

	while (!constraints.empty()) {
		STRING_PAIR clause = constraints.top();
		constraints.pop();

		string lhs = clause.first;
		string rhs = clause.second;
		string parsed_lhs = ParsingSynonym(lhs);
		string parsed_rhs = ParsingSynonym(rhs);
		string lhs_attr = ParsingSynonymAttribute(lhs);
		string rhs_attr = ParsingSynonymAttribute(rhs);
		string lhs_type = entity_map.at(parsed_lhs);

		STRING_LIST key = *(new STRING_LIST());
		STRINGLIST_SET value = *(new STRINGLIST_SET());

		// add constraint into result map
		if (IsVar(lhs) && !IsVar(rhs)) {
			// synonym=int/string
			key = { parsed_lhs };

			string parsed_string = rhs;
			if (IsString(rhs)) {
				parsed_string = ParsingEntRef(rhs);
			}

			if (IsSameEntityType(lhs_type, lhs_attr)) {
				// store directly
				value = EvaluateAllCall(lhs_type);
				STRING_SET tmp = STRING_SET({ parsed_string });
				RemoveIrrelevant(&value, ConvertSet(tmp), 0);
			}
			else {
				// convert 
				value = GetAlternateResult(parsed_string, lhs_type);
			}
		}
		else if (IsVar(lhs) && IsVar(rhs)) {
			//synonym=synonym
			key = { parsed_lhs, parsed_rhs };
			
			string rhs_type = entity_map.at(parsed_rhs);
			STRINGLIST_SET lhs_value = EvaluateAllCall(lhs_type);
			STRINGLIST_SET rhs_value = EvaluateAllCall(rhs_type);
			STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();
			STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();

			if (!IsSameEntityType(entity_map.at(parsed_lhs), lhs_attr)) {
				for (STRING_LIST* s : lhs_value) {
					STRING_SET v = ConvertSet(GetAlternateResult(s->at(0), lhs_type));

					lhs_w_attr.insert({ s->at(0), v });
				}
			}

			if (!IsSameEntityType(entity_map.at(parsed_rhs), rhs_attr)) {
				for (STRING_LIST* s : rhs_value) {
					STRING_SET v = ConvertSet(GetAlternateResult(s->at(0), rhs_type));

					rhs_w_attr.insert({ s->at(0), v });
				}
			}

			if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
				// comparing lhs and rhs
				for (STRING_LIST* lhs_s : lhs_value) {
					for (STRING_LIST* rhs_s : rhs_value) {
						if (lhs_s->at(0).compare(rhs_s->at(0)) == 0) {
							STRING_LIST* to_insert = new STRING_LIST();
							to_insert->push_back(lhs_s->at(0));
							to_insert->push_back(lhs_s->at(0));

							if (!IsDuplicate(value, *to_insert)) {
								value.insert(to_insert);
							}
						}
					}
				}
			}
			else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
				// compare lhs_w_attr and rhs_w_attr
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
						string lhs_key = (*f).first;
						string rhs_key = (*g).first;
						STRING_SET lhs_v = (*f).second;
						STRING_SET rhs_v = (*g).second;

						STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

						if (!val.empty()) {
							STRING_LIST* to_insert = new STRING_LIST();
							to_insert->push_back(lhs_key);
							to_insert->push_back(rhs_key);

							if (!IsDuplicate(value, *to_insert)) {
								value.insert(to_insert);
							}
						}
					}
				}
			}
			else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
				// comparing lhs_w_attr and rhs
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					string key = (*f).first;
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(v, rhs_value, 0);

					for (string s : val) {
						STRING_LIST* to_insert = new STRING_LIST();
						to_insert->push_back(key);
						to_insert->push_back(s);

						if (!IsDuplicate(value, *to_insert)) {
							value.insert(to_insert);
						}
					}	
				}
			}
			else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
				// comparing lhs and rhs_w_attr
				for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
					string key = (*f).first;
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(v, lhs_value, 0);

					for (string s : val) {
						STRING_LIST* to_insert = new STRING_LIST();
						to_insert->push_back(s);
						to_insert->push_back(key);

						if (!IsDuplicate(value, *to_insert)) {
							value.insert(to_insert);
						}
					}
				}
			}			
		}

		if (value.empty()) {
			return false;
		}

		AddResult(key, value, results_map);
	}
	
	return true;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateOneSynonymSet(string f_call, string param) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateOneSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}
	
	PKB pkb;
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

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
		result = ConvertSet(rm.GetCalls(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		result = ConvertSet(rm.GetCallsStars(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		result = ConvertSet(cfgm.GetNext(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		result = ConvertSet(cfgm.GetNextStar(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		result = ConvertSet(am.GetAffects(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		result = ConvertSet(am.GetAffectsStar(ParsingStmtRef(param)));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateOneSynonymSet: No such relref." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateOneSynonymSet: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateInverseOneSynonymSet(string f_call, string param) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateInverseOneSynonymSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}
	
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());
	
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
		result = ConvertSet(rm.GetInverseCalls(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		result = ConvertSet(rm.GetInverseCallsStars(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		result = ConvertSet(cfgm.GetInverseNext(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		result = ConvertSet(cfgm.GetInverseNextStar(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		// result = ConvertSet(cfgm.GetInverseAffects(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		// result = ConvertSet(cfgm.GetInverseAffectsStar(ParsingStmtRef(param)));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateInverseOneSynonymSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateInverseOneSynonymSet: Result clause's size = " << result.size() << endl;
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateTwoSynonymSet(string f_call) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet" << endl;
		cout << "fcall: " << f_call << endl;
	}
	
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
	PKB::AffectsManager am = pkb.GetAffectsManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());
	
	if (UNIT_TESTING) {
		cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet - UNIT TESTING" << endl;
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
		result = ConvertSet(rm.GetAllCalls());
	}
	else if (f_call.compare(TYPE_COND_CALLS_T) == 0) {
		result = ConvertSet(rm.GetAllCallsStar());
	}
	else if (f_call.compare(TYPE_COND_NEXT) == 0) {
		result = ConvertSet(cfgm.GetAllNext());
	}
	else if (f_call.compare(TYPE_COND_NEXT_T) == 0) {
		result = ConvertSet(cfgm.GetAllNextStar());
	}
	else if (f_call.compare(TYPE_COND_AFFECTS) == 0) {
		
		result = ConvertSet(am.GetAllAffects());
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		result = ConvertSet(am.GetAllAffectsStar());
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateTwoSynonymSet: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::EvaluateAllCall(string output_var_type) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateAllCall" << endl;
		cout << "output_var_type: " << output_var_type << endl;
	}
	
	PKB pkb = PKB();
	DataManager dm = pkb.GetDataManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());
	
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
			cout << "PQLOptimizedEvaluator - EvaluateAllCall: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - EvaluateAllCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRING_SET PQLOptimizedEvaluator::ConvertSet(STRINGLIST_SET result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRING_SET final_result = *(new STRING_SET());

	if (!result_set.empty()) {
		for (STRING_LIST* k : result_set) {
			final_result.insert(k->at(0));
		}
	}
	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(STRING_SET result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (string k : result_set) {
			STRING_LIST* value = new STRING_LIST();
			value->push_back(k);

			final_result.insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(INTEGER_SET result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (int k : result_set) {
			STRING_LIST* value = new STRING_LIST();
			value->push_back(to_string(k));

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(DOUBLE_SET result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (double k : result_set) {
			char convert_to_str[50];
			sprintf(convert_to_str, "%.0f", k);

			STRING_LIST* value = new STRING_LIST();
			value->push_back(convert_to_str);

			final_result.insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(STMT_STMT_PAIR_LIST result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STMT_STMT_PAIR k : result_set) {
			string first = to_string(k.s1);
			string second = to_string(k.s2);

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(STMT_VAR_PAIR_LIST result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (STMT_VAR_PAIR k : result_set) {
			string first = to_string(k.s);
			string second = k.v;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(PROC_VAR_PAIR_LIST result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (PROC_VAR_PAIR k : result_set) {
			string first = k.p;
			string second = k.v;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::ConvertSet(PROC_PROC_PAIR_LIST result_set) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - ConvertSet" << endl;
	}

	STRINGLIST_SET final_result = *(new STRINGLIST_SET());

	if (!result_set.empty()) {
		for (PROC_PROC_PAIR k : result_set) {
			string first = k.p1;
			string second = k.p2;

			STRING_LIST* value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result.insert(value);
		}
	}

	return final_result;
}

STRING_SET PQLOptimizedEvaluator::GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, int pos_to_check) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetIntersectResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : val2) {
		for (string j : val1) {
			if ((*i).at(pos_to_check) == j) {
				result.insert(j);
			}
		}
	}

	return result;
}

STRING_SET PQLOptimizedEvaluator::GetIntersectResult(STRING_SET val1, STRING_SET val2) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetIntersectResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	if (val1.size() > val2.size()) {
		for (auto i = val2.begin(); i != val2.end(); i++) {
			if (val1.find(*i) != val1.end())
				result.insert(*i);
		}
	}
	else {
		for (auto i = val1.begin(); i != val1.end(); i++) {
			if (val2.find(*i) != val2.end())
				result.insert(*i);
		}
	}
	

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetIntersectResult(STRINGLIST_SET val1, STRINGLIST_SET val2) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetIntersectResult" << endl;
	}

	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (val1.size() > val2.size()) {
		for (STRING_LIST* i : val2) {
			for (STRING_LIST* j : val1) {
				if (*i == *j) {
					result.insert(i);
				}
			}
		}
	}
	else {
		for (STRING_LIST* i : val1) {
			for (STRING_LIST* j : val2) {
				if (*i == *j) {
					result.insert(i);
				}
			}
		}
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetAlternateResult(string values, string type) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetAlternateResult" << endl;
	}

	RelationManager rm = PKB().GetRelationManager();
	DataManager dm = PKB().GetDataManager();

	STRINGLIST_SET results = *(new STRINGLIST_SET());

	if (type.compare(TYPE_STMT_PRINT) == 0) {
		if (IsInteger(values)) {
			results = ConvertSet(rm.GetStmtUses(ParsingStmtRef(values)));
			RemoveIrrelevant(&results, EvaluateAllCall(TYPE_VAR), 0);
		}
		else {
			results = ConvertSet(rm.GetInverseStmtUses(ParsingEntRef(values)));
			RemoveIrrelevant(&results, EvaluateAllCall(TYPE_STMT_PRINT), 0);
		}

	}
	else if (type.compare(TYPE_STMT_READ) == 0) {
		if (IsInteger(values)) {
			results = ConvertSet(rm.GetStmtModifies(ParsingStmtRef(values)));
			RemoveIrrelevant(&results, EvaluateAllCall(TYPE_VAR), 0);
		}
		else {
			results = ConvertSet(rm.GetInverseStmtModifies(ParsingEntRef(values)));
			RemoveIrrelevant(&results, EvaluateAllCall(TYPE_STMT_READ), 0);
		}
	}
	else {
		// calls
		if (IsInteger(values)) {
			string new_value = dm.GetCalledByStmt(ParsingStmtRef(values));
			if (new_value.compare("") != 0) {
				results.insert(new STRING_LIST({ new_value }));
			}
		}
		else {
			results = ConvertSet(dm.GetInverseCalledBy(ParsingEntRef(values)));
		}
	}
	return results;
}

STRING_SET PQLOptimizedEvaluator::GetAlternateResult(STRINGLIST_SET values, string type) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetAlternateResult" << endl;
	}

	RelationManager rm = PKB().GetRelationManager();
	STRING_SET results = *(new STRING_SET());

	for (STRING_LIST* v : values) {
		STRING_SET new_values = ConvertSet(GetAlternateResult(v->at(0), type));
		results.insert(new_values.begin(), new_values.end());
	}

	return results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetAlternateOutputResult(STRING_SET values, string type) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetAlternateOutputResult" << endl;
	}
	RelationManager rm = PKB().GetRelationManager();
	STRINGLIST_SET results = *(new STRINGLIST_SET());

	for (string v : values) {	
		STRING_SET new_values = ConvertSet(GetAlternateResult(v, type));

		// Combine the values
		for (string s : new_values) {
			STRING_LIST r = STRING_LIST();
			r.push_back(v);
			r.push_back(s);
			
			if (!IsDuplicate(results, r)) {
				results.insert(new STRING_LIST(r));
			}
		}
		
	}

	return results;
}

bool PQLOptimizedEvaluator::RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, int pos_to_check) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - RemoveIrrelevant" << endl;
	}

	bool is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		string v = (*it)->at(pos_to_check);
		bool has_value = false;
		for (STRING_LIST* tmp_entry : tmp) {
			string check = tmp_entry->at(0);
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

bool PQLOptimizedEvaluator::RemoveIrrelevant(STRING_SET* value, STRINGLIST_SET tmp) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - RemoveIrrelevant" << endl;
	}

	bool is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		string v = (*it);
		bool has_value = false;
		for (STRING_LIST* tmp_entry : tmp) {
			string check = tmp_entry->at(0);
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

INTEGERPAIR_SET PQLOptimizedEvaluator::GetCommonSynonymsIndex(STRING_LIST large_keys, STRING_LIST small_keys) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - RemoveIrrelevant" << endl;
	}

	INTEGERPAIR_SET results = *(new INTEGERPAIR_SET());

	for (int s_index = 0; s_index < small_keys.size(); s_index++) {
		for (int l_index = 0; l_index < large_keys.size(); l_index++) {
			if (small_keys[s_index].compare(large_keys[l_index]) == 0) {
				INTEGER_PAIR* pair = new INTEGER_PAIR();
				// l_key = s_key
				pair->first = l_index;
				pair->second = s_index;
				results.insert(pair);
				// should not have duplicated key
				break;
			}
		}
	}

	return results;
}

int PQLOptimizedEvaluator::GetCommonSynonymsIndex(STRING_LIST large_keys, string synonym) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - RemoveIrrelevant" << endl;
	}

	int results = *(new int());
	
	for (int l_index = 0; l_index < large_keys.size(); l_index++) {
		if (synonym.compare(large_keys[l_index]) == 0) {
			results = l_index;
			// should not have duplicated key
			break;
		}
	}

	return results;
}

STRING_SET PQLOptimizedEvaluator::GetNewResult(STRINGLIST_SET value, int pos_to_check) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNewResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : value) {
		result.insert((*i).at(pos_to_check));
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetNewResult(STRINGLIST_SET value, INTEGER_LIST pos_to_check) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNewResult" << endl;
	}

	STRINGLIST_SET result = *(new STRINGLIST_SET());

	for (STRING_LIST* entry : value) {
		STRING_LIST* entry_value = new STRING_LIST();

		for (int j = 0; j < pos_to_check.size(); j++) {
			// Found value to be included in the result
			entry_value->push_back(entry->at(pos_to_check.at(j)));
		}

		if (!IsDuplicate(result, *entry_value)) {
			result.insert(entry_value);
		}
	}

	return result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetCombinedResult(STRINGLIST_SET large_values, STRINGLIST_SET small_values, INTEGERLIST_LIST indexes) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNewResult" << endl;
	}

	STRINGLIST_SET results = *(new STRINGLIST_SET());

	if (large_values.size() < small_values.size()) {
		STRINGLIST_SET tmp = large_values;
		large_values = small_values;
		small_values = tmp;
	}

	for (STRING_LIST* s_entry : small_values) {
		for (STRING_LIST* l_entry: large_values) {
			bool is_common = true;
			INTEGER_SET index_to_exclude = INTEGER_SET();

			// Check if the common synonyms is of the same value
			for (INTEGER_LIST index_entry : indexes) {
				if (l_entry->at(index_entry[0]) != s_entry->at(index_entry[1])) {
					is_common = false;
					break;
				}
				else {
					index_to_exclude.insert(index_entry[1]);
				}
			}

			// Same value -> merge both values, but don't duplicate the common synonyms
			if (is_common) {
				STRING_LIST* entry = new STRING_LIST();
				entry = l_entry;

				// Prevent duplicates
				for (int s = 0; s < s_entry->size(); s++) {
					if (index_to_exclude.find(s) == index_to_exclude.end()) {
						entry->push_back(s_entry->at(s));
					}
				}

				if (!IsDuplicate(results, *entry)) {
					results.insert(entry);
				}
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetCombinedResult(STRINGLIST_SET output_result, STRINGLIST_SET result, int pos_to_compare) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNewResult" << endl;
	}

	STRINGLIST_SET combined_result = STRINGLIST_SET();
	for (auto result_entry = output_result.cbegin(); result_entry != output_result.cend(); result_entry++) {
		STRING_LIST entry = *(*result_entry);
		for (auto r = result.cbegin(); r != result.cend(); r++) {
			STRING_LIST converted_entry = *(*r);

			if (entry.at(pos_to_compare).compare(converted_entry.at(0)) == 0) {
				STRING_LIST val = STRING_LIST();
				for (int i = 0; i < entry.size(); i++) {
					if (i != pos_to_compare) {
						val.push_back(entry.at(i));
					}
					else {
						val.push_back(converted_entry.at(1));
					}

				}

				if (!IsDuplicate(combined_result, val)) {
					combined_result.insert(new STRING_LIST(val));
				}
			}
		}
	}

	return combined_result;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetCartesianProduct(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list, STRING_STRING_MAP entity_map) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetCartesianProduct" << endl;
	}
	
	STRINGLIST_SET results = *(new STRINGLIST_SET());
	STRING_LIST added_output = STRING_LIST();
	for (string output : output_list) {
		string parsed_output = ParsingSynonym(output);
		string output_attr = ParsingSynonymAttribute(output);
		string output_type = entity_map.at(parsed_output);

		for (auto entry = results_map.cbegin(); entry != results_map.cend(); entry++) {
			STRING_LIST* synonyms = (*entry).first;
			STRINGLIST_SET values = (*entry).second;
			bool is_tuple = synonyms->size() > 1 ? true : false;

			for (int i = 0; i < synonyms->size(); i++) {
				if (synonyms->at(i).compare(parsed_output) == 0) {
					// output synonym found
					
					// Check constraint if synonym has already been added in output list
					// Added = dependency, else no
					INTEGERPAIR_SET added_index = INTEGERPAIR_SET();
					for (int s = 0; s < synonyms->size(); s++) {
						for (int a = 0; a < added_output.size(); a++) {
							if (synonyms->at(s).compare(added_output.at(a)) == 0) {
								INTEGER_PAIR* pair = new INTEGER_PAIR();
								pair->first = a;
								pair->second = s;

								added_index.insert(pair);
							}
						}
					}

					if (added_index.empty()) {
						// Get the values to be added
						if (is_tuple) {
							values = ConvertSet(GetNewResult(values, i));
						}
							
						// check attribute
						if (!IsSameEntityType(output_type, output_attr)) {
							values = ConvertSet(GetAlternateResult(values, output_type));
						}

						results = GetNoDependencyProduct(results, values);
					}
					else {
						// Merge with respect to dependencies
						results = GetDependencyProduct(results, values, i, added_index, output_type, output_attr);
					}
					added_output.push_back(parsed_output);
					break;
				}
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values, int pos_to_add, INTEGERPAIR_SET to_check, string output_type, string output_attr) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetDependencyProduct" << endl;
	}

	STRINGLIST_SET final_results = *(new STRINGLIST_SET());

	if (results.empty()) {
		results = ConvertSet(GetNewResult(values, pos_to_add));
	}
	else {
		for (STRING_LIST* set : results) {
			for (STRING_LIST* val : values) {
				if (IsDependencyStatisfied(*set, *val, to_check)) {
					STRING_LIST new_value = *(new STRING_LIST());
					new_value.insert(new_value.end(), set->begin(), set->end());

					STRING_SET tmp = STRING_SET();
					if (pos_to_add != -1) {
						// check attribute 
						if (output_type.compare("") != 0 && output_attr.compare("") != 0 && !IsSameEntityType(output_type, output_attr)) {
							// convert
							tmp = ConvertSet(GetAlternateResult(val->at(pos_to_add), output_type));
							Print(tmp);
						}
						else {
							// add directly
							new_value.push_back(val->at(pos_to_add));
						}
					}

					if (!tmp.empty()) {
						// converted: add 1 by 1
						for (string s : tmp) {
							// keep refreshing state
							STRING_LIST tmp_value = new_value;
							tmp_value.push_back(s);

							if (!IsDuplicate(final_results, tmp_value)) {
								final_results.insert(new STRING_LIST(tmp_value));
							}
						}
					}
					else {
						// add directly
						if (!IsDuplicate(final_results, new_value)) {
							final_results.insert(new STRING_LIST(new_value));
						}
					}
				}
			}
		}
	}

	return final_results;
}

STRINGLIST_SET PQLOptimizedEvaluator::GetNoDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values) {
	if (DEBUG) {
		cout << "PQLOptimizedEvaluator - GetNoDependencyProduct" << endl;
	}

	STRINGLIST_SET final_results = *(new STRINGLIST_SET());

	if (results.empty()) {
		final_results = values;
	}
	else {
		for (STRING_LIST* set : results) {
			for (STRING_LIST* val : values) {
				STRING_LIST* new_value = new STRING_LIST();
				new_value->insert(new_value->end(), set->begin(), set->end());
				new_value->insert(new_value->end(), val->begin(), val->end());

				if (!IsDuplicate(final_results, *new_value)) {
					final_results.insert(new_value);
				}
			}
		}
	}

	return final_results;
}

bool PQLOptimizedEvaluator::IsVar(string var) {
	return IsString(var) || IsInteger(var) || IsUnderscore(var) ? false : true;
}

bool PQLOptimizedEvaluator::IsString(string var) {
	return var[0] == '\"' && var[var.length() - 1] == '\"' ? true : false;
}

bool PQLOptimizedEvaluator::IsInteger(string var) {
	return !var.empty() && find_if(var.begin(),
		var.end(), [](unsigned char c) { return !isdigit(c); }) == var.end();
}

bool PQLOptimizedEvaluator::IsUnderscore(string var) {
	return var.compare("_") == 0 ? true : false;
}

bool PQLOptimizedEvaluator::IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list) {
	for (string synonym : synonyms) {
		for (string output : output_list) {
			if (synonym.compare(ParsingSynonym(output)) == 0) {
				return true;
			}
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsOutputSynonyms(STRING_SET synonyms, STRING_LIST output_list) {
	for (string output : output_list) {
		if (synonyms.find(ParsingSynonym(output)) != synonyms.end()) {
			return true;
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsConstraintSynonym(STRING_SET synonyms, STRING_SET constraint_synonyms) {
	for (string s: synonyms) {
		if (constraint_synonyms.find(s) != constraint_synonyms.end()) {
			return true;
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsDuplicate(STRINGLIST_SET set, STRING_LIST value) {
	for (STRING_LIST* lst : set) {
		if (*lst == value) {
			return true;
		}
	}

	return false;
}

bool PQLOptimizedEvaluator::IsDependencyStatisfied(STRING_LIST result1, STRING_LIST result2, INTEGERPAIR_SET to_check) {
	if (to_check.empty()) {
		return true;
	}
	else {
		for (INTEGER_PAIR* pair : to_check) {
			int r_index = pair->first;
			int v_index = pair->second;

			if (result1.at(r_index) != result2.at(v_index)) {
				return false;
			}
		}
	}

	return true;
}

bool PQLOptimizedEvaluator::IsSameEntityType(string type, string check) {	
	STRING_SET list_of_stmts = { TYPE_STMT_CALL, TYPE_STMT_PRINT, TYPE_STMT_READ };
	if (check.empty()) {
		return true;
	}

	if (list_of_stmts.find(type) != list_of_stmts.end() && check.compare("stmt#") != 0) {
		// stmt# but asking for names
		return false;
	}

	return true;
}

int PQLOptimizedEvaluator::ParsingStmtRef(string param) {
	return param.compare("_") == 0 || param.compare("") == 0 ? -1 : stoi(param);
}

string PQLOptimizedEvaluator::ParsingEntRef(string param) {
	if (param.compare("_") == 0) {
		return "";
	}
	else {
		// trim away the quotation marks
		param.erase(remove(param.begin(), param.end(), '\"'), param.end());
		return param;
	}
}

string PQLOptimizedEvaluator::ParsingSynonym(string synonym) {
	int index = synonym.find(".");
	return index != string::npos ? synonym.substr(0, index) : synonym;
}

string PQLOptimizedEvaluator::ParsingSynonymAttribute(string synonym) {
	int index = synonym.find(".");
	return index != string::npos ? synonym.substr(index + 1, synonym.length()) : "";
}

void PQLOptimizedEvaluator::IncreaseOccurrenceCount(string target, STRING_INTEGER_MAP* occurrence_count) {
	if (occurrence_count->find(target) != occurrence_count->end()) {
		occurrence_count->at(target) += 1;
	}
	else {
		occurrence_count->insert({ target, 1 });
	}
}
