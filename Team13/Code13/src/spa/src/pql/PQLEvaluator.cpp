#include "PQLEvaluator.h"
#include <iostream>

#include "pkb/PKB.h"

QueryResult PQLEvaluator::Evaluate(QueryInfo query_info) {
	STRINGLIST_SET no_user_set = STRINGLIST_SET();
	STRINGLIST_SET one_user_set = STRINGLIST_SET();
	STRINGLIST_SET two_user_set = STRINGLIST_SET();
	STRINGLIST_SET pattern_set = STRINGLIST_SET();
	STRING_STRINGSET_MAP one_user_result_set = STRING_STRINGSET_MAP();
	STRINGLIST_STRINGLISTSET_MAP two_user_result_set = STRINGLIST_STRINGLISTSET_MAP();
	STRING_STRINGSET_MAP consolidated_results = STRING_STRINGSET_MAP();
	QueryResult final_result = QueryResult();

	STRING_STRING_MAP var_map = query_info.GetVarMap();
	STRING_STRINGLISTLIST_MAP rel_ref_map = query_info.GetRelRefMap();
	STRING output_var = query_info.GetOutputVar();
	STRING output_var_type = var_map.at(output_var);

	if (!rel_ref_map.empty()) {
		// Parse clauses conditions
		for (auto f = rel_ref_map.cbegin(); f != rel_ref_map.cend(); f++) {
			STRING f_call = (*f).first;
			STRINGLIST_LIST allParams = (*f).second;

			for (STRING_LIST p : allParams) {
				STRING_LIST* set = new STRING_LIST();
				set->push_back(f_call);
				set->push_back(p[0]);
				set->push_back(p[1]);

				if (set->empty()) {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Parsing clauses: Error creating value for sets." << endl;
					}

					final_result.SetResult({});
					return final_result;
				}

				BOOLEAN is_insert_successful = true;
				if (f_call.compare(TYPE_COND_PATTERN_P) == 0 || f_call.compare(TYPE_COND_PATTERN_F) == 0) {
					// PATTERN clause = Special case
					set->push_back(p[2]);
					if (set->size() != 4) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Error creating value for pattern sets." << endl;
						}
						final_result.SetResult({});
						return final_result;
					}

					if (pattern_set.insert(set).second == 0) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Error inserting into pattern set." << endl;
						}

						is_insert_successful = false;
					}
				}
				else {
					if (!IsVar(p[0], var_map) && !IsVar(p[1], var_map)) {
						// both params != user declared var
						if (no_user_set.insert(set).second == 0) {
							// error
							if (DEBUG) {
								cout << "PQLEvaluator - Parsing clauses: Error inserting no_user_set." << endl;
							}

							is_insert_successful = false;
						}
					}
					else if ((IsVar(p[0], var_map) && !IsVar(p[1], var_map)) || (!IsVar(p[0], var_map) && IsVar(p[1], var_map))) {
						// 1 param == user declared var
						if (one_user_set.insert(set).second == 0) {
							// error
							if (DEBUG) {
								cout << "PQLEvaluator - Parsing clauses: Error inserting one_user_set." << endl;
							}

							is_insert_successful = false;
						}
					}
					else if (IsVar(p[0], var_map) && IsVar(p[1], var_map)) {
						// 2 params = user declared var
						if (two_user_set.insert(set).second == 0) {
							// error
							if (DEBUG) {
								cout << "PQLEvaluator - Parsing clauses: Error inserting two_user_set." << endl;
							}

							is_insert_successful = false;
						}
					}
					else {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Invalid parameters." << endl;
						}
						is_insert_successful = false;
					}
				}

				if (!is_insert_successful) {
					final_result.SetResult({});
					return final_result;
				}
			}
		}

		// Evaluate no_user_set -> T/F clauses
		// FALSE -> return empty; ALL TRUE -> continue
		for (const STRING_LIST* func : no_user_set) {
			STRING f_call = (*func)[0];
			STRING param1 = IsVar((*func)[1], var_map) ? "" : (*func)[1];
			STRING param2 = IsVar((*func)[2], var_map) ? "" : (*func)[2];

			BOOLEAN is_true = true;

			if (!EvaluateNoUserDeclaredSet(f_call, param1, param2)) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating no user declared clauses: False clause." << endl;
				}

				is_true = false;
			}

			if (!is_true) {
				final_result.SetResult({});
				return final_result;
			}
		}

		// Evaluate one_user_set
		// EMPTY set -> return empty;
		for (const STRING_LIST* func : one_user_set) {
			STRING key = "";
			STRING_SET value = STRING_SET();

			STRING f_call = (*func)[0];
			STRING param1 = (*func)[1];
			STRING param2 = (*func)[2];

			if (IsVar(param1, var_map)) {
				// getInverseXXX() branch
				key = param1;

				value = EvaluateInverseOneDeclaredSet(f_call, param2);
			}
			else if (IsVar(param2, var_map)) {
				// getXXX() branch
				key = param2;

				value = EvaluateOneDeclaredSet(f_call, param1);
			}
			else {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating one user declared clauses: No user declared variable." << endl;
				}
				final_result.SetResult({});
				return final_result;
			}

			if (value.empty()) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating one user declared clauses: Empty result set." << endl;
				}
				final_result.SetResult({});
				return final_result;
			}

			STRING_SET tmp = GetAllSet(var_map.at(key));
			RemoveIrrelevant(&value, tmp);
			if (value.empty()) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating one user declared clauses: Empty result set after checking against entity type." << endl;
				}
				final_result.SetResult({});
				return final_result;
			}

			// Check if KEY exists
			// TRUE -> AND the results; FALSE -> insert the results
			if (one_user_result_set.find(key) == one_user_result_set.end()) {
				one_user_result_set.insert({ key, value });
			}
			else {
				one_user_result_set.at(key) = GetIntersectResult(one_user_result_set.at(key), value);
			}
		}

		// Evaluate two_user_set
		// EMPTY set -> return empty
		for (const STRING_LIST* func : two_user_set) {
			STRING_LIST* key = new STRING_LIST();
			STRINGLIST_SET value = STRINGLIST_SET();

			STRING f_call = (*func)[0];
			STRING param1 = (*func)[1];
			STRING param2 = (*func)[2];

			if (param1.compare(param2) == 0) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating two user declared clauses: Same parameters." << endl;
				}
				final_result.SetResult({});
				return final_result;
			}

			key->push_back(param1);
			key->push_back(param2);


			value = EvaluateTwoDeclaredSet(f_call);
			if (value.empty()) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating two user declared clauses: Empty result set." << endl;
				}
				final_result.SetResult({});
				return final_result;
			}

			STRING_SET tmp = GetAllSet(var_map.at(param1));
			RemoveIrrelevant(&value, tmp, 0);

			tmp = GetAllSet(var_map.at(param2));
			RemoveIrrelevant(&value, tmp, 1);

			if (value.empty()) {
				if (DEBUG) {
					cout << "PQLEvaluator - Evaluating two user declared clauses: Empty result set after checking against entity type." << endl;
				}
				final_result.SetResult({});
				return final_result;
			}

			// Check if KEY exists
			// TRUE -> AND the results; FALSE -> insert the results
			STRING_LIST* check_key = IsKey(*key, two_user_result_set);
			if (check_key == nullptr) {
				two_user_result_set.insert({ key, value });
			}
			else {
				two_user_result_set.at(check_key) = GetIntersectResult(two_user_result_set.at(check_key), value);
			}
		}

		// Evaluate pattern_set
		// EMPTY set -> return empty;
		for (const STRING_LIST* func : pattern_set) {
			STRING f_call = (*func)[0];
			STRING param1 = (*func)[1];
			STRING param2 = (*func)[2];
			STRING param3 = (*func)[3];

			if (IsVar(param1, var_map)) {
				// insert into two_user_result_set
				STRING_LIST* key = new STRING_LIST();
				key->push_back(param3);
				key->push_back(param1);

				STRINGLIST_SET value = EvaluateAssignPatternCall(f_call, param2);
				if (value.empty()) {
					if (DEBUG) {
						cout << "PQLEvaluator - Evaluating pattern clauses: Empty result set (1 param)." << endl;
					}

					final_result.SetResult({});
					return final_result;
				}

				// Check if KEY exists
				// TRUE -> AND the results; FALSE -> insert the results
				STRING_LIST* check_key = IsKey(*key, two_user_result_set);
				if (check_key == nullptr) {
					two_user_result_set.insert({ key, value });
				}
				else {
					two_user_result_set.at(check_key) = GetIntersectResult(two_user_result_set.at(check_key), value);
				}
			}
			else {
				// insert into one_user_result_set
				STRING key = param3;
				STRING_SET value = EvaluateAssignPatternCall(f_call, param1, param2);
				if (value.empty()) {
					if (DEBUG) {
						cout << "PQLEvaluator - Evaluating pattern clauses: Empty result set (2 params)." << endl;
					}

					final_result.SetResult({});
					return final_result;
				}

				// Check if KEY exists
				// TRUE -> AND the results; FALSE -> insert the results
				if (one_user_result_set.find(key) == one_user_result_set.end()) {
					one_user_result_set.insert({ key, value });
				}
				else {
					one_user_result_set.at(key) = GetIntersectResult(one_user_result_set.at(key), value);
				}
			}
		}

		// Check if output_var is CONST type
		// TRUE -> return all const vals; FALSE -> continue
		if (output_var_type.compare(TYPE_CONST) == 0) {

			STRING_SET result = *(new STRING_SET(GetAllSet(output_var_type)));
			final_result.SetResult(result);

			return final_result;
		}

		// Consolidate results
		consolidated_results = one_user_result_set;
		if (two_user_result_set.size() != 0) {
			STRING_SET relatedVar = STRING_SET();
			consolidated_results = ConsolidateResults(output_var, relatedVar, consolidated_results, one_user_result_set, two_user_result_set);
		}/*
		else {
			consolidated_results = one_user_result_set;
		}*/
	}

	// Check if output_var is in consolidatedResults
	// YES -> return corresponding values AND getALLXXX(output_var_type); NO -> getALLXXX(output_var_type)
	STRING_SET result;
	if (consolidated_results.find(output_var) != consolidated_results.end()) {
		STRING_SET r1 = GetAllSet(output_var_type);
		STRING_SET r2 = consolidated_results.at(output_var);
		STRING_SET inter_result = GetIntersectResult(r1, r2);
		result = *(new STRING_SET(inter_result));
	}
	else {
		STRING_SET inter_result = GetAllSet(output_var_type);
		result = *(new STRING_SET(inter_result));

	}

	// Return result as QueryResult
	final_result.SetResult(result);
	return final_result;
}

STRING_SET PQLEvaluator::EvaluateAssignPatternCall(STRING f_call, STRING param1, STRING param2) {
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRING_SET result = *(new STRING_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall (2 params to pass)" << endl;
		cout << "PQLEvaluator - EvaluateAssignPatternCall: fcall: " << f_call << "; param: " << param1 << "; param2: " << param2 << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall (2 params to pass) - UNIT TESTING" << endl;
		return { "1", "2", "3" };
	}

	if (f_call.compare(TYPE_COND_PATTERN_P) == 0) {
		result = ConvertSet(pm.GetAssignWithSubPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		result = ConvertSet(pm.GetAssignWithFullPattern(ParsingEntRef(param1), ParsingEntRef(param2)));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern call." << endl;
		}
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall: Result set's size = " << result.size() << endl;
	}
	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateAssignPatternCall(STRING f_call, STRING param) {
	PKB pkb;
	PatternManager pm = pkb.GetPatternManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall (1 param to pass)" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
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
		result = ConvertSet(pm.GetAssignStmtVarPairWithSubPattern("", ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_PATTERN_F) == 0) {
		result = ConvertSet(pm.GetAssignStmtVarPairWithFullPattern("", ParsingEntRef(param)));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateAssignPatternCall: No such pattern call." << endl;
		}
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateAssignPatternCall: Result set's size = " << result.size() << endl;
	}
	return result;
}

BOOLEAN PQLEvaluator::EvaluateNoUserDeclaredSet(STRING f_call, STRING param1, STRING param2) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateNoUserDeclaredSet" << endl;
		cout << "fcall: " << f_call << "; param1: " << param1 << "; param2: " << param2 << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateNoUserDeclaredSet - UNIT TESTING" << endl;
		return true;
	}

	if (f_call.compare(TYPE_COND_FOLLOWS) == 0) {
		cout << "Parsed param 1: " << ParsingStmtRef(param1) << endl;
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
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateNoUserDeclaredSet: No such relref." << endl;
		}

		return false;
	}
}

STRING_SET PQLEvaluator::EvaluateOneDeclaredSet(STRING f_call, STRING param) {
	PKB pkb;
	RelationManager rm = pkb.GetRelationManager();
	STRING_SET result = *(new STRING_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateOneDeclaredSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateOneDeclaredSet - UNIT TESTING" << endl;
		if (f_call.compare(TYPE_COND_FOLLOWS) == 0 || f_call.compare(TYPE_COND_FOLLOWS_T) == 0 ||
			f_call.compare(TYPE_COND_PARENT) == 0 || f_call.compare(TYPE_COND_PARENT_T) == 0) {
			return { "1", "2", "3" };
		}
		else {
			return { "v1","v2","v3" };
		}
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
		cout << "Size: " << rm.GetStmtUses(ParsingStmtRef(param)).size() << endl;
		result = rm.GetStmtUses(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		cout << "Size: " << rm.GetProcUses(ParsingEntRef(param)).size() << endl;
		result = rm.GetProcUses(ParsingEntRef(param));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		result = rm.GetStmtModifies(ParsingStmtRef(param));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = rm.GetProcModifies(ParsingEntRef(param));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateOneDeclaredSet: No such relref." << endl;
		}

		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateOneDeclaredSet: Result set's size = " << result.size() << endl;
	}
	return result;
}

STRING_SET PQLEvaluator::EvaluateInverseOneDeclaredSet(STRING f_call, STRING param) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	STRING_SET result = *(new STRING_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet - UNIT TESTING" << endl;
		if (f_call.compare(TYPE_COND_FOLLOWS) == 0 || f_call.compare(TYPE_COND_FOLLOWS_T) == 0 ||
			f_call.compare(TYPE_COND_PARENT) == 0 || f_call.compare(TYPE_COND_PARENT_T) == 0) {
			return { "1", "2", "3" };
		}
		else {
			return { "v1","v2","v3" };
		}
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
		cout << "Size: " << rm.GetInverseStmtUses(ParsingEntRef(param)).size() << endl;
		result = ConvertSet(rm.GetInverseStmtUses(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_USES_P) == 0) {
		cout << "Size: " << rm.GetInverseProcUses(ParsingEntRef(param)).size() << endl;
		result = rm.GetInverseProcUses(ParsingEntRef(param));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_S) == 0) {
		cout << "Parsed param : " << ParsingEntRef(param) << endl;
		result = ConvertSet(rm.GetInverseStmtModifies(ParsingEntRef(param)));
	}
	else if (f_call.compare(TYPE_COND_MODIFIES_P) == 0) {
		result = rm.GetInverseProcModifies(ParsingEntRef(param));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet: Result set's size = " << result.size() << endl;
	}

	return result;
}

STRINGLIST_SET PQLEvaluator::EvaluateTwoDeclaredSet(STRING f_call) {
	PKB pkb = PKB();
	RelationManager rm = pkb.GetRelationManager();
	STRINGLIST_SET result = *(new STRINGLIST_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateTwoDeclaredSet" << endl;
		cout << "fcall: " << f_call << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - EvaluateTwoDeclaredSet - UNIT TESTING" << endl;
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
		cout << "Size: " << rm.GetAllStmtUses().size() << endl;
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
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateTwoDeclaredSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateTwoDeclaredSet: Result set's size = " << result.size() << endl;
	}
	return result;
}

STRING_STRINGSET_MAP PQLEvaluator::ConsolidateResults(STRING curr_check, STRING_SET related_var, STRING_STRINGSET_MAP consolidated_results,
	STRING_STRINGSET_MAP one_user_result_set, STRINGLIST_STRINGLISTSET_MAP two_user_result_set) {

	if (DEBUG) {
		cout << "PQLEvaluator - ConsolidateResults" << endl;
	}

	BOOLEAN is_changed = false;

	for (auto r2 = two_user_result_set.cbegin(); r2 != two_user_result_set.cend(); r2++) {

		STRING_LIST key = *((*r2).first);
		STRINGLIST_SET value = (*r2).second;
		STRING to_insert = "";
		INTEGER pos_to_check = -1;

		// Check if KEY contains the entity being checked
		// TRUE -> add its other entity into (PENDING) related var
		// FALSE -> go next loop
		if (key[0].compare(curr_check) == 0) {
			to_insert.append(key[1]);
			pos_to_check = 0;
		}
		else if (key[1].compare(curr_check) == 0) {
			to_insert.append(key[0]);
			pos_to_check = 1;
		}
		else {
			continue;
		}

		// Check if one_user_result_set contains entity being checked
		// TRUE -> AND the results
		// FALSE -> create new entry and add all the values
		// BOTH -> insert results into consolidatedResults
		STRING_SET tmp = STRING_SET();
		if (consolidated_results.find(curr_check) != consolidated_results.end()) {
			STRING_SET r1 = consolidated_results.at(curr_check);
			tmp = GetIntersectResult(r1, value, pos_to_check);

			// Remove irrelevant data values from both sets
			// If either removes data, add (PENDING) related var into relatedVar
			if (RemoveIrrelevant(&value, tmp, pos_to_check) || RemoveIrrelevant(&r1, tmp)) {
				is_changed = true;
			}
		}
		else {
			tmp = GetNewResult(value, pos_to_check);
			is_changed = true;
		}

		if (consolidated_results.find(curr_check) == consolidated_results.end()) {
			consolidated_results.insert({ curr_check, *(new STRING_SET(tmp)) });
		}
		else {
			consolidated_results.at(curr_check) = *(new STRING_SET(tmp));
		}

		if (is_changed) {
			related_var.insert(to_insert);
		}
	}

	if (!related_var.empty()) {
		auto to_remove = related_var.begin();
		curr_check = *to_remove;
		related_var.erase(to_remove);
		ConsolidateResults(curr_check, related_var, consolidated_results, one_user_result_set, two_user_result_set);
	}

	return consolidated_results;
}

STRING_SET PQLEvaluator::GetAllSet(STRING output_var_type) {
	PKB pkb = PKB();
	DataManager dm = pkb.GetDataManager();
	STRING_SET result = *(new STRING_SET());

	if (DEBUG) {
		cout << "PQLEvaluator - GetAllSet" << endl;
		cout << "output_var_type: " << output_var_type << endl;
	}

	if (UNIT_TESTING) {
		cout << "PQLEvaluator - GetAllSet - UNIT TESTING" << endl;
		if (output_var_type == TYPE_VAR) {
			result = { "v1", "v2", "v3" };
		}
		else if (output_var_type == TYPE_PROC) {
			result = { "p1", "p2", "p3" };
		}
		else {
			result = { "1", "2", "3" };

		}

		return result;
	}

	if (output_var_type.compare(TYPE_STMT) == 0) {
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
		result = dm.GetAllVariables();
	}
	else if (output_var_type.compare(TYPE_PROC) == 0) {
		result = dm.GetAllProcedures();
	}
	else if (output_var_type.compare(TYPE_CONST) == 0) {
		result = ConvertSet(dm.GetAllConstants());
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - GetAllSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - GetAllSet: Result set's size = " << result.size() << endl;
	}
	return result;
}

STATEMENT_TYPE PQLEvaluator::GetStmtType(STRING output_var_type) {
	if (output_var_type.compare(TYPE_STMT_ASSIGN)) {
		return assignStatement;
	}
	else if (output_var_type.compare(TYPE_STMT_IF)) {
		return ifStatement;
	}
	else {
		return whileStatement;
	}
}

STRING_SET PQLEvaluator::GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, INTEGER pos_to_check) {
	STRING_SET result = *(new STRING_SET());

	for (STRING_LIST* i : val2) {
		for (STRING j : val1) {
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
		if (val2.find(*i) != val2.end()) {
			result.insert(*i);
		}
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


BOOLEAN PQLEvaluator::RemoveIrrelevant(STRING_SET* value, STRING_SET tmp) {
	BOOLEAN is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		if (tmp.find(*it) == tmp.end()) {
			it = value->erase(it);
			is_changed = true;
		}
		else {
			it++;
		}
	}

	return is_changed;
}

BOOLEAN PQLEvaluator::RemoveIrrelevant(STRINGLIST_SET* value, STRING_SET tmp, INTEGER pos_to_check) {
	BOOLEAN is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		STRING v = (*it)->at(pos_to_check);
		if (tmp.find(v) == tmp.end()) {
			it = value->erase(it);
			is_changed = true;
		}
		else {
			++it;
		}
	}

	return is_changed;
}

STRING_SET PQLEvaluator::ConvertSet(INTEGER_SET result_set) {
	STRING_SET final_result = *(new STRING_SET());

	if (!result_set.empty()) {
		for (INTEGER k : result_set) {
			final_result.insert(to_string(k));
		}
	}
	return final_result;
}

STRING_SET PQLEvaluator::ConvertSet(DOUBLE_SET result_set) {
	STRING_SET final_result = *(new STRING_SET());

	if (!result_set.empty()) {
		for (DOUBLE k : result_set) {
			final_result.insert(to_string(((INTEGER)(k))));
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


STRING_LIST* PQLEvaluator::IsKey(STRING_LIST key, STRINGLIST_STRINGLISTSET_MAP two_user_result_set) {
	for (auto k = two_user_result_set.cbegin(); k != two_user_result_set.cend(); k++) {
		STRING_LIST* existing_key = (*k).first;

		if (key == *existing_key) {
			return existing_key;
		}
	}

	return nullptr;
}

BOOLEAN PQLEvaluator::IsVar(STRING var, STRING_STRING_MAP var_map) {
	if (var_map.find(var) == var_map.end()) {
		return false;
	}
	else {
		return true;
	}
}

BOOLEAN PQLEvaluator::IsWildCard(STRING var) {
	if (var.compare("_") == 0) {
		return true;
	}
	else {
		return false;
	}
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
