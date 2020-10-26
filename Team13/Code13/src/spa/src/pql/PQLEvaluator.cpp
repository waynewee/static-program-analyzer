#include "PQLEvaluator.h"
#include <iostream>

#include "pkb/PKB.h"

QueryResult PQLEvaluator::Evaluate(QueryInfo query_info) {
	// Final result to be returned
	STRINGLIST_SET final_result_set = *(new STRINGLIST_SET());

	// Expected output list
	STRING_LIST output_list = query_info.GetOutputList();
	bool is_boolean_output = query_info.IsBooleanOutput();

	// Unevaluated clauses & constraints
	STRINGSET_STRINGLISTSET_MAP synonyms_map = STRINGSET_STRINGLISTSET_MAP();
	STRINGPAIR_SET constraints = STRINGPAIR_SET();

	// Intermediate results from evaluated clauses
	STRINGLIST_STRINGLISTSET_MAP results_map = STRINGLIST_STRINGLISTSET_MAP();
	STRINGLIST_STRINGLISTSET_MAP final_results_map = STRINGLIST_STRINGLISTSET_MAP();

	// Query Info object
	STRING_STRING_MAP entity_map = query_info.GetEntityMap();

	// Parse clauses conditions
	bool has_parsed = true;
	
	if (!ParseClauses(query_info, &constraints, &results_map)) {
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
		return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
	}
	
	// Evaluate synonyms_map: 1 related-synonyms group at a time
	for (auto f = synonyms_map.cbegin(); f != synonyms_map.cend(); f++) {
		STRINGLIST_STRINGLISTSET_MAP tmp_map = results_map;
		STRINGLIST_SET clauses = (*f).second;

		for (STRING_LIST* c : clauses) {
			string f_call = (*c)[0];
			string param1 = (*c)[1];
			string param2 = (*c)[2];

			STRING_LIST c_key = STRING_LIST();
			STRINGLIST_SET c_value = STRINGLIST_SET();
			
			if (f_call.compare(TYPE_COND_PATTERN_F) == 0 || f_call.compare(TYPE_COND_PATTERN_P) == 0) {
				string param3 = (*c)[3];
				if (c->size() == 5) {
					param3 = (*c)[4];
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
					cout << "PQLEvaluator - Evaluating synonyms_map: Empty results." << endl;
				}

				return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
			}

			// Add into the tmp_map
			AddResult(c_key, c_value, &tmp_map);
		}

		// Check if any of the synonyms is in expected output list
		// or if the constraints not checked: "TRUE" -> add into results_map; "FALSE" -> continue
		for (auto tmp_entry = tmp_map.cbegin(); tmp_entry != tmp_map.cend(); tmp_entry++) {
			// Should only have 1 entry
			STRING_LIST* keys = (*tmp_entry).first;
			STRINGLIST_SET values = (*tmp_entry).second;

			// Check constraints
			bool is_dependency_checked = CheckConstraints(&constraints, entity_map, &results_map, *keys, &values);

			if (!is_dependency_checked) {
				AddResult(*keys, values, &results_map);
			}
			else {
				if (values.empty()) {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Checked constraints: Empty results." << endl;
					}

					return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
				}

				if (IsOutputSynonyms(*keys, output_list)) {
					AddResult(*keys, values, &results_map);
				}
			}			
		}
		
	}

	if (!is_boolean_output) {
		// Get output_list's results from results_map
		INTEGER_SET retrieved_output_index = INTEGER_SET();

		for (auto result_entry = results_map.cbegin(); result_entry != results_map.cend(); result_entry++) {
			STRING_LIST* result_key = (*result_entry).first;
			STRING_LIST* key = new STRING_LIST();
			INTEGER_LIST index_to_extract = INTEGER_LIST();

			for (int i = 0; i < result_key->size(); i++) {
				for (int o = 0; o < output_list.size(); o++) {
					string parsed_output_synonym = ParsingSynonym(output_list.at(o));

					if (retrieved_output_index.find(o) == retrieved_output_index.end()) {

						if (result_key->at(i).compare(parsed_output_synonym) == 0) {
							key->push_back(output_list.at(o));

							// Add index of value to extract to list
							index_to_extract.push_back(i);

							// Add index of retrieved output synonym to list
							retrieved_output_index.insert(o);
						}
					}
				}

				if (retrieved_output_index.size() == output_list.size()) {
					break;
				}
			}

			if (!key->empty()) {
				// Has >=1 expected output synonym(s)
				STRING_LIST output_key = *(new STRING_LIST());
				STRINGLIST_SET output_result = GetNewResult((*result_entry).second, index_to_extract);

				for (int k = 0; k < key->size(); k++) {
					string synonym = ParsingSynonym(key->at(k));
					string synonym_type = ParsingSynonymAttribute(key->at(k));

					output_key.push_back(synonym);

					// Check if output synonym is attribute type
					STRINGLIST_SET result = STRINGLIST_SET();
					if (!IsSameEntityType(entity_map.at(synonym), synonym_type)) {
						result = GetAlternateOutputResult(GetNewResult(output_result, k), synonym_type);
					}

					if (!result.empty()) {
						// Combine results
						output_result = GetCombinedResult(output_result, result, k);
					}
				}

				AddResult(output_key, output_result, &final_results_map);
			}
		}

		// Get all output_list's results that are not in results_map
		// STRINGLIST_STRINGLISTSET_MAP tmp_map = STRINGLIST_STRINGLISTSET_MAP();
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

					// Check constraints
					bool is_dependency_checked = CheckConstraints(&constraints, entity_map, &results_map, key, &value);
					if (is_dependency_checked && value.empty()) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Checked constraints: Empty results." << endl;
						}

						return SetResult(is_boolean_output, "FALSE", *(new STRINGLIST_SET()));
					}

					if (!IsSameEntityType(entity_map.at(parsed_synonym), synonym_type)) {
						value = ConvertSet(GetAlternateResult(value, entity_map.at(parsed_synonym)));
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
	}

	// PRINT CHECK WITH 
	cout << "CHECKING WITH CLAUSE AT THE END" << endl;
	Print(constraints);
/*
	// Check the independent constraints: not captured in clauses / output synonyms
	STRINGPAIR_SET checked = STRINGPAIR_SET();
	for (STRING_PAIR* check : constraints) {
		string lhs_synonym = check->first;
		string rhs_synonym = check->second;

		string parsed_lhs = ParsingSynonym(lhs_synonym);
		string parsed_rhs = ParsingSynonym(rhs_synonym);

		string lhs_attr = ParsingSynonymAttribute(lhs_synonym);
		string rhs_attr = ParsingSynonymAttribute(rhs_synonym);

		if (IsVar(parsed_lhs) && !IsVar(parsed_rhs)) {
			// Synonym = int/str
			string lhs_type = entity_map.at(parsed_lhs);
			STRINGLIST_SET lhs = EvaluateAllCall(lhs_type);
			STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();

			if (!IsSameEntityType(lhs_type, lhs_attr)) {
				for (STRING_LIST* s : lhs) {
					STRING_SET v = GetAlternateResult(s->at(0), lhs_type);

					lhs_w_attr.insert({ s->at(0), v });

				}
			}

			STRING_SET rhs = STRING_SET();
			IsString(rhs_synonym) ? rhs.insert(ParsingEntRef(rhs_synonym)) : rhs.insert(rhs_synonym);

			if (lhs_w_attr.empty()) {
				// compare lhs and str/int
				STRINGLIST_SET val = ConvertSet(GetIntersectResult(rhs, lhs, 0));

				if (val.empty()) {
					// empty result
					return SetResult(is_boolean_output, "FALSE", {});
				}
				else {
					checked.insert(check);
				}
			}
			else {
				// compare lhs attr and str/int

				STRING_SET lhs_val = STRING_SET();
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(rhs, v);
					lhs_w_attr.at((*f).first) = val;

					if (!val.empty()) {
						lhs_val.insert((*f).first);
					}
				}

				RemoveIrrelevant(&lhs, ConvertSet(lhs_val), 0);

				if (lhs.empty()) {
					// empty result
					return SetResult(is_boolean_output, "FALSE", {});
				}
				else {
					checked.insert(check);
				}
			}
		}
		else if (IsVar(parsed_lhs) && IsVar(parsed_rhs)) {
			// synonym=synonym

			string lhs_type = entity_map.at(parsed_lhs);
			STRINGLIST_SET lhs = EvaluateAllCall(lhs_type);
			STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();

			if (!IsSameEntityType(lhs_type, lhs_attr)) {
				for (STRING_LIST* s : lhs) {
					STRING_SET v = GetAlternateResult(s->at(0), lhs_type);

					lhs_w_attr.insert({ s->at(0), v });

				}
			}

			string rhs_type = entity_map.at(parsed_rhs);
			STRINGLIST_SET rhs = EvaluateAllCall(rhs_type);
			STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();

			if (!IsSameEntityType(rhs_type, rhs_attr)) {
				for (STRING_LIST* s : rhs) {
					STRING_SET v = GetAlternateResult(s->at(0), rhs_type);

					rhs_w_attr.insert({ s->at(0), v });

				}
			}

			if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
				// compare lhs and rhs
				STRINGLIST_SET val = GetIntersectResult(lhs, rhs);
				if (val.empty()) {
					return SetResult(is_boolean_output, "FALSE", {});
				}
				else {
					checked.insert(check);
				}
			}
			else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
				// compare lhs and rhs attr

				STRING_SET lhs_val = STRING_SET();
				STRING_SET rhs_val = STRING_SET();
				for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(v, lhs, 0);
					rhs_w_attr.at((*f).first) = val;
					lhs_val.insert(val.begin(), val.end());

					if (!val.empty()) {
						rhs_val.insert((*f).first);
					}
				}

				if (lhs_val.empty() || rhs_val.empty()) {
					return SetResult(is_boolean_output, "FALSE", {});
				}
				else {
					checked.insert(check);
				}
			}
			else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
				// compare lhs attr and rhs

				STRING_SET lhs_val = STRING_SET();
				STRING_SET rhs_val = STRING_SET();
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(v, rhs, 0);
					lhs_w_attr.at((*f).first) = val;
					rhs_val.insert(val.begin(), val.end());

					if (!val.empty()) {
						lhs_val.insert((*f).first);
					}
				}

				if (lhs_val.empty() || rhs_val.empty()) {
					return SetResult(is_boolean_output, "FALSE", {});
				}
				else {
					checked.insert(check);
				}
			}
			else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
				// compare lhs attr and rhs attr

				STRING_SET lhs_val = STRING_SET();
				STRING_SET rhs_val = STRING_SET();
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
						STRING_SET lhs_v = (*f).second;
						STRING_SET rhs_v = (*g).second;

						STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

						if (!val.empty()) {
							lhs_val.insert((*f).first);
							rhs_val.insert((*g).first);
						}
					}
				}

				if (lhs_val.empty() || rhs_val.empty()) {
					return SetResult(is_boolean_output, "FALSE", {});
				}
				else {
					checked.insert(check);
				}
			}
		}
	}

	for (STRING_PAIR* s : checked) {
		constraints.erase(s);
	}
	
	// PRINT CHECK WITH 
	cout << "CHECKING WITH CLAUSE AT THE END end " << endl;
	Print(constraints);
*/
	if (!is_boolean_output) {
		// Combine all the expected output results
		final_result_set = GetCartesianProduct(final_results_map, output_list);
	}

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
	return SetResult(is_boolean_output, "TRUE", final_result_set);
}

void PQLEvaluator::Print(STRINGPAIR_SET to_print) {
	for (STRING_PAIR* p : to_print) {
		cout << "<" << p->first << "=" << p->second << "> ";
	}
	cout << endl;
}

void PQLEvaluator::Print(STRINGLIST_STRINGLISTSET_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		for (string s : *(*result_entry).first) {
			cout << s << " ";
		}
		cout << ": ";;
		Print((*result_entry).second);
	}
}

void PQLEvaluator::Print(STRINGSET_STRINGLISTSET_MAP to_print) {
	for (auto result_entry = to_print.cbegin(); result_entry != to_print.cend(); result_entry++) {
		Print(*(*result_entry).first);
		cout << ": ";
		Print((*result_entry).second);
	}
}

void PQLEvaluator::Print(STRING_SET to_print) {
	for (auto f = to_print.cbegin(); f != to_print.cend(); f++) {
		cout << (*f) << " ";
	}
	cout << endl;
}

void PQLEvaluator::Print(STRING_LIST to_print) {
	cout << "{ ";
	for (string s : to_print) {
		cout << s << " ";
	}
	cout << "} ";
}

void PQLEvaluator::Print(STRINGLIST_SET to_print) {
	for (STRING_LIST* set : to_print) {
		Print(*set);
	}
	cout << endl;
}

void PQLEvaluator::AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP* results_map) {
	STRING_LIST* check_key = GetRelatedSynonyms(key, *results_map);
	if (check_key == nullptr) {
		// not related (yet)
		results_map->insert({ new STRING_LIST(key), value });
	}
	else {
		// Merge common synonyms & their results
		
		INTEGERPAIR_SET index_to_compare = INTEGERPAIR_SET();
		INTEGER_SET pos_to_ignore = INTEGER_SET();
		for (int s = 0; s < check_key->size(); s++) {
			for (int a = 0; a < key.size(); a++) {
				if (check_key->at(s).compare(key.at(a)) == 0) {
					INTEGER_PAIR* pair = new INTEGER_PAIR();
					pair->first = s;
					pair->second = a;

					index_to_compare.insert(pair);
					pos_to_ignore.insert(a);
				}
			}
		}

		STRINGLIST_SET tmp_result = results_map->at(check_key);
		STRING_LIST new_key = *(new STRING_LIST(*check_key));
		
		if (pos_to_ignore.size() == key.size()) {
			tmp_result = GetDependencyProduct(tmp_result, value, -1, index_to_compare);
		}
		else {
			for (int i = 0; i < key.size(); i++) {
				if (pos_to_ignore.find(i) == pos_to_ignore.end()) {

					tmp_result = GetDependencyProduct(tmp_result, value, i, index_to_compare);

					new_key.push_back(key.at(i));
				}
			}
		}
		
		results_map->erase(check_key);
		results_map->insert({ new STRING_LIST(new_key), tmp_result });
	}
}

QueryResult PQLEvaluator::SetResult(bool is_boolean_output, string bool_result, STRINGLIST_SET result) {
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

bool PQLEvaluator::ParseClauses(QueryInfo query_info, STRINGPAIR_SET* constraints, STRINGLIST_STRINGLISTSET_MAP* results_map) {
	if (DEBUG) {
		cout << "PQLEvaluator - ParseClauses: WITH" << endl;
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
						cout << "PQLEvaluator - Parsing clauses: Error inserting constraints." << endl;
					}
					return false;
				}

				string parsed_lhs = ParsingSynonym(lhs);
				string parsed_rhs = ParsingSynonym(rhs);

				STRING_LIST key = *(new STRING_LIST());
				STRINGLIST_SET value = *(new STRINGLIST_SET());

				// add constraint into result map
				if (IsVar(lhs) && !IsVar(rhs)) {
					// synonym=int/string
					key = { parsed_lhs };
					if (IsString(rhs)) {
						value.insert(new STRING_LIST({ ParsingEntRef(rhs) }));
					}
					else {
						value.insert(new STRING_LIST({ rhs }));
					}
				}
				else if (IsVar(lhs) && IsVar(rhs)) {
					//synonym=synonym
					key = { parsed_lhs, parsed_rhs };
					
					STRINGLIST_SET lhs_value = EvaluateAllCall(query_info.GetEntityMap().at(parsed_lhs));
					STRINGLIST_SET rhs_value = EvaluateAllCall(query_info.GetEntityMap().at(parsed_rhs));
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
				
				AddResult(key, value, results_map);
				
			}
		}
		
	}

	return true;
}

bool PQLEvaluator::ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map) {
	STRING_STRINGLISTLIST_MAP st_map = query_info.GetStMap();
	STRING_STRINGLISTLIST_MAP pattern_map = query_info.GetPatternMap();
	// STRING_INTEGER_MAP occurrence_count = *(new STRING_INTEGER_MAP());

	for (auto f = st_map.cbegin(); f != st_map.cend(); f++) {
		string f_call = (*f).first;
		STRINGLIST_LIST all_params = (*f).second;

		for (STRING_LIST p : all_params) {
			if (p[0].compare(p[1]) == 0 && !IsUnderscore(p[0]) && f_call.compare(TYPE_COND_NEXT_T) != 0) {
				// same parameter & not _ & not Next*
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
				string param1 = IsVar(clause->at(1)) ? "" : clause->at(1);
				string param2 = IsVar(clause->at(2)) ? "" : clause->at(2);

				if (!EvaluateNoSynonymSet(f_call, param1, param2)) {
					if (DEBUG) {
						cout << "PQLEvaluator - Evaluating no synonym clauses: False clause." << endl;
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
					value.insert(clause);
					synonyms_map->at(key) = value;
				}
				else {
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
					cout << "PQLEvaluator - Parsing clauses: Error creating value for pattern clauses." << endl;
				}

				return false;
			}

			STRING_SET* key = new STRING_SET();
			key->insert(type);

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
				value.insert(clause);
				synonyms_map->at(key) = value;
			}
			else {
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
	}

	return true;
}

STRING_LIST* PQLEvaluator::GetRelatedSynonyms(string synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
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

STRING_LIST* PQLEvaluator::GetRelatedSynonyms(STRING_LIST synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map) {
	for (auto k = synonyms_map.cbegin(); k != synonyms_map.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		for (string existing_synonym: *existing_key) {
			// found related synonyms in the synonyms_map
			for (string checking_synonym: synonym) {
				if (existing_synonym.compare(checking_synonym) == 0) {
					return existing_key;
				}
			}
		}
	}

	return nullptr;
}

STRING_SET* PQLEvaluator::GetRelatedSynonyms(string synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map) {
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
	bool has_related_synonyms = false;

	STRING_SET* new_keys = new STRING_SET();
	STRINGLIST_SET new_values = *(new STRINGLIST_SET());

	for (auto k = synonyms.cbegin(); k != synonyms.cend(); k++) {
		string curr_check = *k;
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

bool PQLEvaluator::EvaluateNoSynonymSet(string f_call, string param1, string param2) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();

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
		// return rm.IsAffects(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		// return rm.IsAffectsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateNoSynonymSet: No such relref." << endl;
		}

		return false;
	}
}

STRINGLIST_SET PQLEvaluator::EvaluatePatternCall(string f_call, string param1, string param2, string type) {
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluatePatternCall (2 params to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param1 << "; param2: " << param2 << "; type: " << type << endl;
	}

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluatePatternCall: No such pattern type." << endl;
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
				cout << "PQLEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluatePatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluatePatternCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluatePatternCall(string f_call, string param, string type) {
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluatePatternCall (1 param to pass)" << endl;
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
		if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithSubPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		if (type.compare(TYPE_STMT_IF) == 0) {
			result = ConvertSet(pm.GetIfStmtVarPairWithPattern(""));
		}
		else if (type.compare(TYPE_STMT_WHILE) == 0) {
			cout << "reached" << endl;
			result = ConvertSet(pm.GetWhileStmtVarPairWithPattern(""));
		}
		else if (type.compare(TYPE_STMT_ASSIGN) == 0) {
			result = ConvertSet(pm.GetAssignStmtVarPairWithFullPattern("", ParsingEntRef(param)));
		}
		else {
			// error 
			if (DEBUG) {
				cout << "PQLEvaluator - EvaluatePatternCall: No such pattern type." << endl;
			}

			return {};
		}
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluatePatternCall: No such pattern call." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluatePatternCall: Result clause's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateOneSynonymSet(string f_call, string param) {
	PKB pkb;
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
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
		// result = ConvertSet(cfgm.GetAffects(ParsingStmtRef(param)));
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		// result = ConvertSet(cfgm.GetAffectsStar(ParsingStmtRef(param)));
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

STRINGLIST_SET PQLEvaluator::EvaluateInverseOneSynonymSet(string f_call, string param) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
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
			cout << "PQLEvaluator - EvaluateInverseOneSynonymSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneSynonymSet: Result clause's size = " << result.size() << endl;
	}

	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateTwoSynonymSet(string f_call) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	CFGManager cfgm = pkb.GetCFGManager();
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
		result = ConvertSet(cfgm.GetAllAffects());
	}
	else if (f_call.compare(TYPE_COND_AFFECTS_T) == 0) {
		result = ConvertSet(cfgm.GetAllAffectsStar());
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

STRINGLIST_SET PQLEvaluator::EvaluateAllCall(string output_var_type) {
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
		for (string k : result_set) {
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
		for (int k : result_set) {
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

STRINGLIST_SET PQLEvaluator::ConvertSet(STMT_STMT_PAIR_LIST result_set) {
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

STRINGLIST_SET PQLEvaluator::ConvertSet(STMT_VAR_PAIR_LIST result_set) {
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

STRINGLIST_SET PQLEvaluator::ConvertSet(PROC_VAR_PAIR_LIST result_set) {
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

STRINGLIST_SET PQLEvaluator::ConvertSet(PROC_PROC_PAIR_LIST result_set) {
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

bool PQLEvaluator::CheckConstraints(STRINGPAIR_SET* constraints, STRING_STRING_MAP entity_map, 
	STRINGLIST_STRINGLISTSET_MAP* results_map, STRING_LIST key, STRINGLIST_SET* value) {
	if (DEBUG) {
		cout << "PQLEvaluator - Check WITH constraints" << endl;
	}
	
	bool is_checked = true;
	
	STRINGPAIR_SET checked = STRINGPAIR_SET();
	for (STRING_PAIR* check : *constraints) {
		string lhs_synonym = check->first;
		string rhs_synonym = check->second;

		string parsed_lhs = ParsingSynonym(lhs_synonym);
		string parsed_rhs = ParsingSynonym(rhs_synonym);

		string lhs_attr = ParsingSynonymAttribute(lhs_synonym);
		string rhs_attr = ParsingSynonymAttribute(rhs_synonym);
		
		INTEGER_PAIR key_with_constraint = make_pair(-1, -1);

		for (int key_index = 0; key_index < key.size(); key_index++) {
			// Check if constraint applies to synonyms
			if (parsed_lhs.compare(key[key_index]) == 0) {
				key_with_constraint.first = key_index;
			}
			else if (parsed_rhs.compare(key[key_index]) == 0) {
				key_with_constraint.second = key_index;
			}
		}

		if (key_with_constraint.first == -1 && key_with_constraint.second == -1) {
			// not applicable constraint
			continue;
		}
		else if (key_with_constraint.first != -1 && key_with_constraint.second == -1) {
			// LHS = synonym, RHS = int/str/synonym@result
			cout << "REACHED key_with_constraint.first != -1 && key_with_constraint.second == -1" << endl;

			STRING_SET lhs = GetNewResult(*value, key_with_constraint.first);
			STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();
			string lhs_type = entity_map.at(parsed_lhs);
			if (!IsSameEntityType(lhs_type, lhs_attr)) {
				for (string s : lhs) {
					STRING_SET v = GetAlternateResult(s, lhs_type);

					lhs_w_attr.insert({ s, v });
				}
			}

			if (IsString(rhs_synonym) || IsInteger(rhs_synonym)) {
				STRING_SET rhs = STRING_SET();

				IsString(rhs_synonym) ? rhs.insert(ParsingEntRef(rhs_synonym)) : rhs.insert(rhs_synonym);
				
				if (lhs_w_attr.empty()) {
					// compare lhs and str/int
					STRINGLIST_SET val = ConvertSet(GetIntersectResult(lhs, rhs));
					RemoveIrrelevant(value, val, key_with_constraint.first);
				}
				else {
					// compare lhs attr and str/int
					STRING_SET lhs_val = STRING_SET();
					for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
						STRING_SET v = (*f).second;
						STRING_SET val = GetIntersectResult(rhs, v);
						lhs_w_attr.at((*f).first) = val;

						if (!val.empty()) {
							lhs_val.insert((*f).first);
						}
					}

					RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
				}
			}
			else {
				STRING_LIST* check_key = GetRelatedSynonyms(parsed_rhs, *results_map);
				if (check_key == nullptr) {
					// not in results yet
					is_checked = false;
					
					string rhs_type = entity_map.at(parsed_rhs);
					STRINGLIST_SET rhs = EvaluateAllCall(rhs_type);
					STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();

					if (!IsSameEntityType(rhs_type, rhs_attr)) {
						for (STRING_LIST* s : rhs) {
							STRING_SET v = GetAlternateResult(s->at(0), rhs_type);

							rhs_w_attr.insert({ s->at(0), v });

						}
					}

					if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs and rhs
						STRING_SET val = GetIntersectResult(lhs, rhs, 0);

						if (DEBUG) {
							cout << "LHS VALUE: " << endl;
							Print(lhs);

							cout << "RHS VALUE: " << endl;
							Print(rhs);

							cout << "INTERSECTED VALUE: " << endl;
							Print(val);
						}

						RemoveIrrelevant(value, ConvertSet(val), key_with_constraint.first);
					}
					else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {

							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(v, lhs);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(lhs);

								cout << "RHS VALUE: " << endl;
								Print(v);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							rhs_w_attr.at((*f).first) = val;
							lhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								rhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
					}
					else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs attr and rhs

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(v, rhs, 0);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(v);

								cout << "RHS VALUE: " << endl;
								Print(rhs);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							lhs_w_attr.at((*f).first) = val;
							rhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								lhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
					}
					else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs attr and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
								STRING_SET lhs_v = (*f).second;
								STRING_SET rhs_v = (*g).second;

								STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

								if (DEBUG) {
									cout << "LHS VALUE: " << endl;
									Print(lhs_v);

									cout << "RHS VALUE: " << endl;
									Print(rhs_v);

									cout << "INTERSECTED VALUE: " << endl;
									Print(val);
								}

								if (!val.empty()) {
									lhs_val.insert((*f).first);
									rhs_val.insert((*g).first);
								}
							}
						}

						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
					}
				}
				else {					
					int index = GetCommonSynonymsIndex(*check_key, parsed_rhs);
					STRING_SET rhs = GetNewResult(results_map->at(check_key), index);
					STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();
					string rhs_type = entity_map.at(parsed_rhs);

					if (!IsSameEntityType(rhs_type, rhs_attr)) {
						for (string s : rhs) {
							STRING_SET v = GetAlternateResult(s, rhs_type);
							rhs_w_attr.insert({ s, v });
						}
					}

					if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs and rhs
						STRINGLIST_SET val = ConvertSet(GetIntersectResult(lhs, rhs));
						RemoveIrrelevant(value, val, key_with_constraint.first);
						RemoveIrrelevant(&results_map->at(check_key), val, index);
					}
					else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(lhs, v);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(lhs);

								cout << "RHS VALUE: " << endl;
								Print(v);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							rhs_w_attr.at((*f).first) = val;
							lhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								rhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
						RemoveIrrelevant(&results_map->at(check_key), ConvertSet(rhs_val), index);
					}
					else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs attr and rhs

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(rhs, v);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(rhs);

								cout << "RHS VALUE: " << endl;
								Print(v);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							lhs_w_attr.at((*f).first) = val;
							rhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								lhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
						RemoveIrrelevant(&results_map->at(check_key), ConvertSet(rhs_val), index);
					}
					else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs attr and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
								STRING_SET lhs_v = (*f).second;
								STRING_SET rhs_v = (*g).second;

								STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

								if (DEBUG) {
									cout << "LHS VALUE: " << endl;
									Print(lhs_v);

									cout << "RHS VALUE: " << endl;
									Print(rhs_v);

									cout << "INTERSECTED VALUE: " << endl;
									Print(val);
								}

								if (!val.empty()) {
									lhs_val.insert((*f).first);
									rhs_val.insert((*g).first);
								}
							}
						}

						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
						RemoveIrrelevant(&results_map->at(check_key), ConvertSet(rhs_val), index);
					}
				}
			}

		}
		else if (key_with_constraint.first == -1 && key_with_constraint.second != -1) {
			// LHS = int/str/synonym@result, RHS = synonym

			cout << "REACHED key_with_constraint.first == -1 && key_with_constraint.second != -1" << endl;

			STRING_SET rhs = GetNewResult(*value, key_with_constraint.second);
			STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();
			string rhs_type = entity_map.at(parsed_rhs);

			if (!IsSameEntityType(rhs_type, rhs_attr)) {
				for (string s : rhs) {
					STRING_SET v = GetAlternateResult(s, rhs_type);
					rhs_w_attr.insert({ s, v });
				}
			}

			if (IsString(lhs_synonym) || IsInteger(lhs_synonym)) {
				cout << "reached string/integer" << endl;

				STRING_SET lhs = STRING_SET();

				IsString(lhs_synonym) ? rhs.insert(ParsingEntRef(lhs_synonym)) : lhs.insert(lhs_synonym);

				if (rhs_w_attr.empty()) {
					// compare str/int and rhs
					STRINGLIST_SET val = ConvertSet(GetIntersectResult(rhs, lhs));
					RemoveIrrelevant(value, val, key_with_constraint.second);
				}
				else {
					// compare str/int and rhs attr 
					STRING_SET rhs_val = STRING_SET();
					for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
						STRING_SET v = (*f).second;
						STRING_SET val = GetIntersectResult(lhs, v);
						rhs_w_attr.at((*f).first) = val;

						if (!val.empty()) {
							rhs_val.insert((*f).first);
						}
					}

					RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
				}
			}
			else {
				STRING_LIST* check_key = GetRelatedSynonyms(parsed_lhs, *results_map);
				if (check_key == nullptr) {
					// not in results yet
					is_checked = false;

					string lhs_type = entity_map.at(parsed_lhs);
					STRINGLIST_SET lhs = EvaluateAllCall(lhs_type);
					STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();

					if (!IsSameEntityType(lhs_type, lhs_attr)) {
						for (STRING_LIST* s : lhs) {
							STRING_SET v = GetAlternateResult(s->at(0), lhs_type);
							lhs_w_attr.insert({ s->at(0), v });
						}
					}

					if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs and rhs
						STRING_SET val = GetIntersectResult(rhs, lhs, 0);
						RemoveIrrelevant(value, ConvertSet(val), key_with_constraint.second);
					}
					else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs attr and rhs

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(v, rhs);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(v);

								cout << "RHS VALUE: " << endl;
								Print(rhs);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							lhs_w_attr.at((*f).first) = val;
							rhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								lhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
					}
					else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(v, lhs, 0);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(lhs);

								cout << "RHS VALUE: " << endl;
								Print(v);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							rhs_w_attr.at((*f).first) = val;
							lhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								rhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
					}
					else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs attr and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
								STRING_SET lhs_v = (*f).second;
								STRING_SET rhs_v = (*g).second;

								STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

								if (DEBUG) {
									cout << "LHS VALUE: " << endl;
									Print(lhs_val);

									cout << "RHS VALUE: " << endl;
									Print(rhs_v);

									cout << "INTERSECTED VALUE: " << endl;
									Print(val);
								}

								if (!val.empty()) {
									lhs_val.insert((*f).first);
									rhs_val.insert((*g).first);
								}
							}
						}

						RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
					}
				}
				else {
					int index = GetCommonSynonymsIndex(*check_key, parsed_lhs);

					string lhs_type = entity_map.at(parsed_lhs);
					STRING_SET lhs = GetNewResult(results_map->at(check_key), index);
					STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();

					if (!IsSameEntityType(lhs_type, lhs_attr)) {
						for (string s : lhs) {
							STRING_SET v = GetAlternateResult(s, lhs_type);
							lhs_w_attr.insert({ s, v });
						}
					}

					if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs and rhs
						STRINGLIST_SET val = ConvertSet(GetIntersectResult(rhs, lhs));
						RemoveIrrelevant(&results_map->at(check_key), val, index);
						RemoveIrrelevant(value, val, key_with_constraint.second);
					}
					else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(lhs, v);
							
							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(lhs);

								cout << "RHS VALUE: " << endl;
								Print(v);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							rhs_w_attr.at((*f).first) = val;
							lhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								rhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(&results_map->at(check_key), ConvertSet(rhs_val), index);
						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.second);
					}
					else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
						// compare lhs attr and rhs

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							STRING_SET v = (*f).second;
							STRING_SET val = GetIntersectResult(rhs, v);

							if (DEBUG) {
								cout << "LHS VALUE: " << endl;
								Print(v);

								cout << "RHS VALUE: " << endl;
								Print(rhs);

								cout << "INTERSECTED VALUE: " << endl;
								Print(val);
							}

							lhs_w_attr.at((*f).first) = val;
							rhs_val.insert(val.begin(), val.end());

							if (!val.empty()) {
								lhs_val.insert((*f).first);
							}
						}

						RemoveIrrelevant(&results_map->at(check_key), ConvertSet(rhs_val), index);
						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.second);
					}
					else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
						// compare lhs attr and rhs attr

						STRING_SET lhs_val = STRING_SET();
						STRING_SET rhs_val = STRING_SET();
						for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
							for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
								STRING_SET lhs_v = (*f).second;
								STRING_SET rhs_v = (*g).second;

								STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

								if (DEBUG) {
									cout << "LHS VALUE: " << endl;
									Print(lhs_v);

									cout << "RHS VALUE: " << endl;
									Print(rhs_v);

									cout << "INTERSECTED VALUE: " << endl;
									Print(val);
								}

								if (!val.empty()) {
									lhs_val.insert((*f).first);
									rhs_val.insert((*g).first);
								}
							}
						}

						RemoveIrrelevant(&results_map->at(check_key), ConvertSet(rhs_val), index);
						RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.second);
					}
				}
			}
		}
		else if (key_with_constraint.first != -1 && key_with_constraint.second != -1) {
			// LHS = RHS = synonym

			cout << "REACHED key_with_constraint.first != -1 && key_with_constraint.second != -1" << endl;

			STRING_SET lhs = GetNewResult(*value, key_with_constraint.first);
			STRING_SET rhs = GetNewResult(*value, key_with_constraint.second);
			STRING_STRINGSET_MAP lhs_w_attr = STRING_STRINGSET_MAP();
			STRING_STRINGSET_MAP rhs_w_attr = STRING_STRINGSET_MAP();
			string lhs_type = entity_map.at(parsed_lhs);
			string rhs_type = entity_map.at(parsed_rhs);
			if (!IsSameEntityType(lhs_type, lhs_attr)) {
				for (string s : lhs) {
					STRING_SET v = GetAlternateResult(s, lhs_type);
					lhs_w_attr.insert({ s, v });
				}
			}

			if (!IsSameEntityType(rhs_type, rhs_attr)) {
				for (string s : rhs) {
					STRING_SET v = GetAlternateResult(s, rhs_type);
					rhs_w_attr.insert({ s, v });
				}
			}

			if (lhs_w_attr.empty() && rhs_w_attr.empty()) {
				// compare lhs and rhs
				STRINGLIST_SET val = ConvertSet(GetIntersectResult(lhs, rhs));
				RemoveIrrelevant(value, val, key_with_constraint.first);
				RemoveIrrelevant(value, val, key_with_constraint.second);
			}
			else if (lhs_w_attr.empty() && !rhs_w_attr.empty()) {
				// compare lhs and rhs attr

				STRING_SET lhs_val = STRING_SET();
				STRING_SET rhs_val = STRING_SET();
				for (auto f = rhs_w_attr.cbegin(); f != rhs_w_attr.cend(); f++) {
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(lhs, v);

					if (DEBUG) {
						cout << "LHS VALUE: " << endl;
						Print(lhs);

						cout << "RHS VALUE: " << endl;
						Print(v);

						cout << "INTERSECTED VALUE: " << endl;
						Print(val);
					}

					rhs_w_attr.at((*f).first) = val;
					lhs_val.insert(val.begin(), val.end());

					if (!val.empty()) {
						rhs_val.insert((*f).first);
					}
				}
				
				RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
				RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
			}
			else if (!lhs_w_attr.empty() && rhs_w_attr.empty()) {
				// compare lhs attr and rhs

				STRING_SET lhs_val = STRING_SET();
				STRING_SET rhs_val = STRING_SET();
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					STRING_SET v = (*f).second;
					STRING_SET val = GetIntersectResult(rhs, v);

					if (DEBUG) {
						cout << "LHS VALUE: " << endl;
						Print(v);

						cout << "RHS VALUE: " << endl;
						Print(rhs);

						cout << "INTERSECTED VALUE: " << endl;
						Print(val);
					}

					lhs_w_attr.at((*f).first) = val;
					rhs_val.insert(val.begin(), val.end());

					if (!val.empty()) {
						lhs_val.insert((*f).first);
					}
				}

				RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
				RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
			}
			else if (!lhs_w_attr.empty() && !rhs_w_attr.empty()) {
				// compare lhs attr and rhs attr

				STRING_SET lhs_val = STRING_SET();
				STRING_SET rhs_val = STRING_SET();
				for (auto f = lhs_w_attr.cbegin(); f != lhs_w_attr.cend(); f++) {
					for (auto g = rhs_w_attr.cbegin(); g != rhs_w_attr.cend(); g++) {
						STRING_SET lhs_v = (*f).second;
						STRING_SET rhs_v = (*g).second;

						STRING_SET val = GetIntersectResult(lhs_v, rhs_v);

						if (DEBUG) {
							cout << "LHS VALUE: " << endl;
							Print(lhs_v);

							cout << "RHS VALUE: " << endl;
							Print(rhs_v);

							cout << "INTERSECTED VALUE: " << endl;
							Print(val);
						}

						if (!val.empty()) {
							lhs_val.insert((*f).first);
							rhs_val.insert((*g).first);
						}
					}
				}

				RemoveIrrelevant(value, ConvertSet(lhs_val), key_with_constraint.first);
				RemoveIrrelevant(value, ConvertSet(rhs_val), key_with_constraint.second);
			}
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLEvaluator: Checking WITH constraint - Error getting constraint synonyms" << endl;
			}

			value->clear();
			break;
		}

		if (is_checked) {
			checked.insert(check);
		}
	}

	for (STRING_PAIR* s : checked) {
		constraints->erase(s);
	}
	
	return is_checked;
}


STRING_SET PQLEvaluator::GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, int pos_to_check) {
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

STRING_SET PQLEvaluator::GetIntersectResult(STRING_SET val1, STRING_SET val2) {
	STRING_SET result = *(new STRING_SET());

	for (auto i = val1.begin(); i != val1.end(); i++) {
		if (val2.find(*i) != val2.end())
			result.insert(*i);
	}

	return result;
}

STRINGLIST_SET PQLEvaluator::GetIntersectResult(STRINGLIST_SET val1, STRINGLIST_SET val2) {
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	for (STRING_LIST* i : val1) {
		for (STRING_LIST* j : val2) {
			if (*i == *j) {
				result.insert(i);
			}
		}
	}

	return result;
}

STRING_SET PQLEvaluator::GetAlternateResult(string values, string type) {
	RelationManager rm = PKB().GetRelationManager();
	DataManager dm = PKB().GetDataManager();

	STRING_SET results = *(new STRING_SET());

	if (type.compare(TYPE_STMT_PRINT) == 0) {
		results = rm.GetStmtUses(ParsingStmtRef(values));
	}
	else if (type.compare(TYPE_STMT_READ) == 0) {
		results = rm.GetStmtModifies(ParsingStmtRef(values));
	}
	else {
		// calls
		string new_value = dm.GetCalledByStmt(ParsingStmtRef(values));
		if (new_value.compare("") != 0) {
			results.insert(new_value);
		}
	}

	return results;
}
/*
STRING_SET PQLEvaluator::GetAlternateResult(STRING_SET values, string type) {
	RelationManager rm = PKB().GetRelationManager();
	STRING_SET results = *(new STRING_SET());

	for (string v : values) {
		STRING_SET new_values = *(new STRING_SET());
		if (type.compare(TYPE_STMT_PRINT) == 0) {
			new_values = rm.GetStmtUses(ParsingStmtRef(v->at(0)));
		}
		else if (type.compare(TYPE_STMT_READ) == 0) {
			new_values = rm.GetStmtModifies(ParsingStmtRef(v->at(0)));
		}
		else {
			// calls
			string new_value = PKB().GetDataManager().GetCalledByStmt(ParsingStmtRef(v->at(0)));
			if (new_value.compare("") != 0) {
				new_values.insert(new_value);
			}
		}

		if (new_values.empty()) {
			results.clear();
			break;
		}

		results.insert(new_values.begin(), new_values.end());
	}

	return results;
}
*/
STRING_SET PQLEvaluator::GetAlternateResult(STRINGLIST_SET values, string type) {
	RelationManager rm = PKB().GetRelationManager();
	STRING_SET results = *(new STRING_SET());

	for (STRING_LIST* v : values) {
		STRING_SET new_values = *(new STRING_SET());
		if (type.compare(TYPE_STMT_PRINT) == 0) {
			new_values = rm.GetStmtUses(ParsingStmtRef(v->at(0)));
		}
		else if (type.compare(TYPE_STMT_READ) == 0) {
			new_values = rm.GetStmtModifies(ParsingStmtRef(v->at(0)));
		} 
		else {
			// calls
			string new_value = PKB().GetDataManager().GetCalledByStmt(ParsingStmtRef(v->at(0)));
			if (new_value.compare("") != 0) {
				new_values.insert(new_value);
			}
		}

		if (new_values.empty()) {
			results.clear();
			break;
		}

		results.insert(new_values.begin(), new_values.end());
	}

	return results;
}

STRINGLIST_SET PQLEvaluator::GetAlternateOutputResult(STRING_SET values, string type) {
	RelationManager rm = PKB().GetRelationManager();
	STRINGLIST_SET results = *(new STRINGLIST_SET());

	for (string v : values) {
		STRING_SET new_values = *(new STRING_SET());
		if (type.compare(TYPE_STMT_READ) == 0 || type.compare(TYPE_STMT_PRINT) == 0) {
			new_values = rm.GetStmtUses(ParsingStmtRef(v));
		}
		else {
			// calls
			string new_value = PKB().GetDataManager().GetCalledByStmt(ParsingStmtRef(v));
			if (new_value.compare("") != 0) {
				new_values.insert(new_value);
			}
		}

		if (new_values.empty()) {
			results.clear();
			break;
		}

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

bool PQLEvaluator::RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, int pos_to_check) {
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

INTEGERPAIR_SET PQLEvaluator::GetCommonSynonymsIndex(STRING_LIST large_keys, STRING_LIST small_keys) {
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

int PQLEvaluator::GetCommonSynonymsIndex(STRING_LIST large_keys, string synonym) {
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

STRING_SET PQLEvaluator::GetNewResult(STRINGLIST_SET value, int pos_to_check) {
	if (DEBUG) {
		cout << "PQLEvaluator - GetNewResult" << endl;
	}

	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : value) {
		result.insert((*i).at(pos_to_check));
	}

	return result;
}

STRINGLIST_SET PQLEvaluator::GetNewResult(STRINGLIST_SET value, INTEGER_LIST pos_to_check) {
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

STRINGLIST_SET PQLEvaluator::GetCombinedResult(STRINGLIST_SET large_values, STRINGLIST_SET small_values, INTEGERLIST_LIST indexes) {
	STRINGLIST_SET results = *(new STRINGLIST_SET());

	for (STRING_LIST* l_entry : large_values) {
		for (STRING_LIST* s_entry : small_values) {
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

STRINGLIST_SET PQLEvaluator::GetCombinedResult(STRINGLIST_SET output_result, STRINGLIST_SET result, int pos_to_compare) {
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

STRINGLIST_SET PQLEvaluator::GetCartesianProduct(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list) {
	if (DEBUG) {
		cout << "PQLEvaluator - GetCartesianProduct" << endl;
		cout << "Results map: " << endl;
		Print(results_map);
	}
	
	STRINGLIST_SET results = *(new STRINGLIST_SET());
	STRING_LIST added_output = STRING_LIST();
	for (string output : output_list) {
		string parsed_output = ParsingSynonym(output);
		for (auto entry = results_map.cbegin(); entry != results_map.cend(); entry++) {
			STRING_LIST* synonyms = (*entry).first;
			STRINGLIST_SET values = (*entry).second;
			bool is_tuple = synonyms->size() > 1 ? true : false;

			for (int i = 0; i < synonyms->size(); i++) {
				if (synonyms->at(i).compare(parsed_output) == 0) {
					// output synonym found
					if (!is_tuple) {
						results = GetNoDependencyProduct(results, values);
					}
					else {
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
							STRINGLIST_SET synonym_values = ConvertSet(GetNewResult(values, i));
							results = GetNoDependencyProduct(results, synonym_values);
						}
						else {
							// Merge with respect to dependencies
							results = GetDependencyProduct(results, values, i, added_index);
						}
					}
					added_output.push_back(output);
					break;
				}
			}
		}
	}

	return results;
}

STRINGLIST_SET PQLEvaluator::GetDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values, int pos_to_add, INTEGERPAIR_SET to_check) {
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

					if (pos_to_add != -1) {
						new_value.push_back(val->at(pos_to_add));
					}

					if (!IsDuplicate(final_results, new_value)) {
						final_results.insert(new STRING_LIST(new_value));
					}
				}
			}
		}
	}

	return final_results;
}

STRINGLIST_SET PQLEvaluator::GetNoDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values) {
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

bool PQLEvaluator::IsVar(string var) {
	return IsString(var) || IsInteger(var) || IsUnderscore(var) ? false : true;
}

bool PQLEvaluator::IsString(string var) {
	return var[0] == '\"' && var[var.length() - 1] == '\"' ? true : false;
}

bool PQLEvaluator::IsInteger(string var) {
	return !var.empty() && find_if(var.begin(),
		var.end(), [](unsigned char c) { return !isdigit(c); }) == var.end();
}

bool PQLEvaluator::IsUnderscore(string var) {
	return var.compare("_") == 0 ? true : false;
}

bool PQLEvaluator::IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list) {
	for (string synonym : synonyms) {
		for (string output : output_list) {
			if (synonym.compare(ParsingSynonym(output)) == 0) {
				return true;
			}
		}
	}

	return false;
}

bool PQLEvaluator::IsDuplicate(STRINGLIST_SET set, STRING_LIST value) {
	for (STRING_LIST* lst : set) {
		if (*lst == value) {
			return true;
		}
	}

	return false;
}

bool PQLEvaluator::IsDependencyStatisfied(STRING_LIST result1, STRING_LIST result2, INTEGERPAIR_SET to_check) {
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

bool PQLEvaluator::IsSameEntityType(string type, string check) {	
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

int PQLEvaluator::ParsingStmtRef(string param) {
	return param.compare("_") == 0 || param.compare("") == 0 ? -1 : stoi(param);
}

string PQLEvaluator::ParsingEntRef(string param) {
	if (param.compare("_") == 0) {
		return "";
	}
	else {
		// trim away the quotation marks
		param.erase(remove(param.begin(), param.end(), '\"'), param.end());
		return param;
	}
}

string PQLEvaluator::ParsingSynonym(string synonym) {
	int index = synonym.find(".");
	return index != string::npos ? synonym.substr(0, index) : synonym;
}

string PQLEvaluator::ParsingSynonymAttribute(string synonym) {
	int index = synonym.find(".");
	return index != string::npos ? synonym.substr(index + 1, synonym.length()) : "";
}
