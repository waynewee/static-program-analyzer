#include "PQLEvaluator.h"

#include <iostream>

#include "pkb/PKB.h"

QueryResult* PQLEvaluator::Evaluate(QueryInfo* query_info) {
	STRINGLIST_SET_PTR no_user_set = new STRINGLIST_SET();
	STRINGLIST_SET_PTR one_user_set = new STRINGLIST_SET();
	STRINGLIST_SET_PTR two_user_set = new STRINGLIST_SET();
	STRING_STRINGSET_MAP_PTR one_user_result_set = new STRING_STRINGSET_MAP();
	STRINGLIST_STRINGLISTSET_MAP_PTR two_user_result_set = new STRINGLIST_STRINGLISTSET_MAP();
	QueryResult* final_result = new QueryResult();

	STRING_STRING_MAP_PTR var_map = query_info->GetVarMap();
	STRING_STRINGLISTLIST_MAP_PTR rel_ref_map = query_info->GetRelRefMap();
	STRING_PTR output_var = query_info->GetOutputVar();
	STRING_PTR output_var_type = var_map->at(output_var);
	
	// Parse clauses conditions
	for (auto f = rel_ref_map->cbegin(); f != rel_ref_map->cend(); f++) {
		string* fCall = (*f).first;
		STRINGLIST_LIST_PTR allParams = (*f).second;
	
		for (STRING_LIST_PTR p : *allParams) {
			STRING_LIST_PTR set = new STRING_LIST();
			set->push_back(fCall);
			set->push_back(p->at(0));
			set->push_back(p->at(1));

			if (set->empty()) {
				// error
				if (DEBUG) {
					cout << "PQLEvaluator - Parsing clauses: Error creating value for sets." << endl;
				}

				final_result->SetResult({});
				return final_result;
			}

			BOOLEAN is_insert_successful = true;
			if (fCall->compare(TYPE_COND_PATTERN) == 0) {
				// PATTERN clause = Special case
				set->push_back(p->at(2));
				if (set->size() != 4) {
					// error
					if (DEBUG) {
						cout << "PQLEvaluator - Parsing clauses: Error creating value for pattern sets." << endl;
					}

					final_result->SetResult({});
					return final_result;
				}

				if (!IsVar(p->at(0), var_map)) {
					// Pattern clause with assign & var
					if (two_user_set->insert(set).second == 0) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Error inserting pattern's two_user_set." << endl;
						}

						is_insert_successful = false;
					}
				}
				else if (IsVar(p->at(0), var_map)) {
					// Pattern clause with assign only
					if (one_user_set->insert(set).second == 0) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Error inserting pattern's one_user_set." << endl;
						}

						is_insert_successful = false;
					}
				}
			}
			else {
				if (output_var_type->compare(TYPE_CONST) == 0 || (!IsVar(p->at(0), var_map) && !IsVar(p->at(1), var_map))) {
					// output_var == CONST type
					// OR
					// both params != user declared var
					if (no_user_set->insert(set).second == 0) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Error inserting no_user_set." << endl;
						}

						is_insert_successful = false;
					}
				}
				else if ((IsVar(p->at(0), var_map) && !IsVar(p->at(1), var_map)) || (!IsVar(p->at(0), var_map) && IsVar(p->at(1), var_map))) {
					// 1 param == user declared var
					if (one_user_set->insert(set).second == 0) {
						// error
						if (DEBUG) {
							cout << "PQLEvaluator - Parsing clauses: Error inserting one_user_set." << endl;
						}

						is_insert_successful = false;
					}
				}
				else if (IsVar(p->at(0), var_map) && IsVar(p->at(1), var_map)) {
					// 2 params = user declared var
					if (two_user_set->insert(set).second == 0) {
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
				final_result->SetResult({});
				return final_result;
			}
		}
	}
	
	// Evaluate no_user_set -> T/F clauses
	// FALSE -> return empty; ALL TRUE -> continue
	for (const STRING_LIST_PTR func : *no_user_set) {
		STRING_PTR f_call = func->at(0);
		STRING_PTR param1 = func->at(1);
		STRING_PTR param2 = func->at(2);

		if (!EvaluateNoUserDeclaredSet(f_call, param1, param2)) {
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating no user declared clauses: Empty result set received." << endl;
			}
			final_result->SetResult({});
			return final_result;
		}
	}
	
	// Evaluate one_user_set
	// EMPTY set -> return empty;
	for (const STRING_LIST_PTR func : *one_user_set) {
		STRING_PTR key = new STRING();
		STRING_SET_PTR value = new STRING_SET();

		STRING_PTR f_call = func->at(0);
		STRING_PTR param1 = func->at(1);
		STRING_PTR param2 = func->at(2);

		if (f_call->compare(TYPE_COND_PATTERN) == 0) {
			// string param3 = (*func)[3];
			// key.assign(param3);

			// PKB pkb = PKB();
			// STATEMENT_TYPE stmtType = GetStmtType(output_var_type);
			// value = pkbGetRelationManager().GetPattern(stmtType, ParsingEntRef(param1), param2);
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating one user declared clauses: pattern not implemented yet." << endl;
			}
		} 
		else if (IsVar(param1, var_map)) {
			// getInverseXXX() branch
			key = param1;

			// value = EvaluateOneDeclaredSet(f_call, param2);
			value = EvaluateInverseOneDeclaredSet(f_call, param2);
		}
		else if (IsVar(param2, var_map)) {
			// getXXX() branch
			key = param2;

			value = EvaluateOneDeclaredSet(f_call, param1);
			// value = EvaluateInverseOneDeclaredSet(f_call, param1);
		}
		else {
			// error
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating one user declared clauses: No user declared variable." << endl;
			}
			final_result->SetResult({});
			return final_result;
		}

		if (value->empty()) {
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating one user declared clauses: Empty result set." << endl;
			}

			final_result->SetResult({});
			return final_result;
		}

		// Check if KEY exists
		// TRUE -> AND the results; FALSE -> insert the results
		if (one_user_result_set->find(key) == one_user_result_set->end()) {
			one_user_result_set->insert({ key, value });
		}
		else {
			one_user_result_set->at(key) = GetIntersectResult(one_user_result_set->at(key), value);
		}
	}
	
	// Evaluate two_user_set
	// EMPTY set -> return empty 
	for (const STRING_LIST_PTR func : *two_user_set) {
		STRING_LIST_PTR key =  new STRING_LIST();
		STRINGLIST_SET_PTR value = new STRINGLIST_SET();

		STRING_PTR f_call = func->at(0);
		STRING_PTR param1 = func->at(1);
		STRING_PTR param2 = func->at(2);

		key->push_back(param1);
		key->push_back(param2);

		if (f_call->compare(TYPE_COND_PATTERN) == 0) {
			// string param3 = (*func)[3];
			// (*key).push_back(param3);
			// (*key).push_back(param1);

			// PKB pkb = PKB();
			// STATEMENT_TYPE stmtType = GetStmtType(output_var_type);
			// value = pkb.GetRelationManager().GetPattern(stmtType, nullptr, param2);
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating two user declared clauses: pattern not implemented yet." << endl;
			}
		}
		else {
			value = EvaluateTwoDeclaredSet(f_call);
		}

		if (value->empty()) {
			if (DEBUG) {
				cout << "PQLEvaluator - Evaluating two user declared clauses: Empty result set." << endl;
			}

			final_result->SetResult({});
			return final_result;
		}

		// Check if KEY exists
		// TRUE -> AND the results; FALSE -> insert the results
		STRING_LIST_PTR checkKey = IsKey(key, two_user_result_set);
		if (checkKey == nullptr) {
			two_user_result_set->insert({ key, value });
		}
		else {
			two_user_result_set->at(checkKey) = GetIntersectResult(two_user_result_set->at(checkKey), value);
		}
	}
	
	// Check if output_var is CONST type
	// TRUE -> return all const vals; FALSE -> continue
	if (output_var_type->compare(TYPE_CONST) == 0) {
		PKB pkb = PKB();

		final_result->SetResult(ConvertSet(pkb.GetDataManager()->GetAllConstants()));
		return final_result;
	}
	
	// Consolidate results
	STRING_STRINGSET_MAP_PTR consolidated_results = new STRING_STRINGSET_MAP();

	if (two_user_result_set->size() != 0) {
		STRING_SET_PTR related_var = new STRING_SET();
		consolidated_results = ConsolidateResults(output_var, related_var, consolidated_results, one_user_result_set, two_user_result_set);
	}
	else {
		consolidated_results = one_user_result_set;
	}

	
	// Check if output_var is in consolidated_results
	// YES -> return corresponding values; NO -> getALLXXX(output_var_type)
	if (consolidated_results->find(output_var) != consolidated_results->end()) {
		final_result->SetResult(consolidated_results->at(output_var));
	}
	else {
		final_result->SetResult(GetAllSet(output_var_type));
	}

	// Return result as QueryResult
	return final_result;
}

BOOLEAN PQLEvaluator::EvaluateNoUserDeclaredSet(STRING_PTR f_call, STRING_PTR param1, STRING_PTR param2) {
	PKB* pkb = new PKB();
	RelationManager* rm = pkb->GetRelationManager();

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateNoUserDeclaredSet" << endl;
		cout << "fcall: " << f_call << "; param1: " << param1 << "; param2: " << param2 << endl;
	}

	if (f_call->compare(TYPE_COND_FOLLOWS) == 0) {
		return rm->IsFollows(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call->compare(TYPE_COND_FOLLOWS_T) == 0) {
		return rm->IsFollowsStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call->compare(TYPE_COND_PARENT) == 0) {
		return rm->IsParent(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call->compare(TYPE_COND_PARENT_T) == 0) {
		return rm->IsParentStar(ParsingStmtRef(param1), ParsingStmtRef(param2));
	}
	else if (f_call->compare(TYPE_COND_USES_S) == 0) {
		return rm->IsStmtUses(ParsingStmtRef(param1), ParsingEntRef(param2));
	}
	else if (f_call->compare(TYPE_COND_USES_P) == 0) {
		return rm->IsProcUses(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_S) == 0) {
		return rm->IsStmtModifies(ParsingStmtRef(param1), ParsingEntRef(param2));
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_P) == 0) {
		return rm->IsProcModifies(ParsingEntRef(param1), ParsingEntRef(param2));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateNoUserDeclaredSet: No such relref." << endl;
		}
	
		return false;
	}
}

STRING_SET_PTR PQLEvaluator::EvaluateOneDeclaredSet(STRING_PTR f_call, STRING_PTR param) {
	PKB pkb;
	RelationManager* rm = pkb.GetRelationManager();
	STRING_SET_PTR result = new STRING_SET();

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateOneDeclaredSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	if (f_call->compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(rm->GetFollows(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(rm->GetFollowsStars(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(rm->GetParents(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(rm->GetParentStars(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm->GetStmtUses(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm->GetProcUses(ParsingEntRef(param)));
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm->GetStmtModifies(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm->GetProcModifies(ParsingEntRef(param)));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateOneDeclaredSet: No such relref." << endl;
		}
		
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateOneDeclaredSet: Result set's size = " << result->size() << endl;
	}

	return result;
}

STRING_SET_PTR PQLEvaluator::EvaluateInverseOneDeclaredSet(STRING_PTR f_call, STRING_PTR param) {
	PKB* pkb = new PKB();
	RelationManager* rm = pkb->GetRelationManager();
	STRING_SET_PTR result = new STRING_SET();

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet" << endl;
		cout << "fcall: " << f_call << "; param: " << param << endl;
	}

	if (f_call->compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(rm->GetInverseFollows(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(rm->GetInverseFollowsStars(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(rm->GetInverseParents(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(rm->GetInverseParentStars(ParsingStmtRef(param)));
	}
	else if (f_call->compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm->GetInverseStmtUses(ParsingEntRef(param)));
	}
	else if (f_call->compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm->GetInverseProcUses(ParsingEntRef(param)));
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm->GetInverseStmtModifies(ParsingEntRef(param)));
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm->GetInverseProcModifies(ParsingEntRef(param)));
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateInverseOneDeclaredSet: Result set's size = " << result->size() << endl;
	}

	return result;
}

STRINGLIST_SET_PTR PQLEvaluator::EvaluateTwoDeclaredSet(STRING_PTR f_call) {
	PKB* pkb = new PKB();
	RelationManager* rm = pkb->GetRelationManager();
	STRINGLIST_SET_PTR result = new STRINGLIST_SET();

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateTwoDeclaredSet" << endl;
		cout << "fcall: " << f_call << endl;
	}	

	if (f_call->compare(TYPE_COND_FOLLOWS) == 0) {
		result = ConvertSet(rm->GetAllFollows());
	}
	else if (f_call->compare(TYPE_COND_FOLLOWS_T) == 0) {
		result = ConvertSet(rm->GetAllFollowsStars());
	}
	else if (f_call->compare(TYPE_COND_PARENT) == 0) {
		result = ConvertSet(rm->GetAllParents());
	}
	else if (f_call->compare(TYPE_COND_PARENT_T) == 0) {
		result = ConvertSet(rm->GetAllParentStars());
	}
	else if (f_call->compare(TYPE_COND_USES_S) == 0) {
		result = ConvertSet(rm->GetAllStmtUses());
	}
	else if (f_call->compare(TYPE_COND_USES_P) == 0) {
		result = ConvertSet(rm->GetAllProcUses());
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_S) == 0) {
		result = ConvertSet(rm->GetAllStmtModifies());
	}
	else if (f_call->compare(TYPE_COND_MODIFIES_P) == 0) {
		result = ConvertSet(rm->GetAllProcModifies());
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - EvaluateTwoDeclaredSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - EvaluateTwoDeclaredSet: Result set's size = " << result->size() << endl;
	}

	return result;
}

STRING_STRINGSET_MAP_PTR PQLEvaluator::ConsolidateResults(STRING_PTR curr_check, STRING_SET_PTR related_var,
	STRING_STRINGSET_MAP_PTR consolidated_results, STRING_STRINGSET_MAP_PTR one_user_result_set,
	STRINGLIST_STRINGLISTSET_MAP_PTR two_user_result_set) {

	if (DEBUG) {
		cout << "PQLEvaluator - ConsolidateResults" << endl;
	}

	BOOLEAN is_changed = false;

	for (auto r2 = two_user_result_set->cbegin(); r2 != two_user_result_set->cend(); r2++) {

		STRING_LIST_PTR key = (*r2).first;
		STRINGLIST_SET_PTR value = (*r2).second;
		STRING_PTR toInsert = new STRING();
		INTEGER pos_to_check = INTEGER();

		// Check if KEY contains the entity being checked
		// TRUE -> add its other entity into (PENDING) related var 
		// FALSE -> go next loop
		if (key->at(0)->compare(*curr_check)) {
			toInsert = key->at(1);
			pos_to_check = 0;
		}
		else if (key->at(1)->compare(*curr_check)) {
			toInsert = key->at(0);
			pos_to_check = 1;
		}
		else {
			continue;
		}

		// Check if one_user_result_set contains entity being checked
		// TRUE -> AND the results
		// FALSE -> create new entry and add all the values 
		// BOTH -> insert results into consolidated_results
		STRING_SET_PTR tmp = new STRING_SET();
		if (one_user_result_set->find(curr_check) != one_user_result_set->end()) {
			STRING_SET_PTR r1 = one_user_result_set->at(curr_check);
			tmp = GetIntersectResult(r1, value, pos_to_check);

			// Remove irrelevant data values from both sets
			// If either removes data, add (PENDING) related var into related_var
			if (RemoveIrrelevant(value, tmp, pos_to_check) || RemoveIrrelevant(r1, tmp)) {
				is_changed = true;
			}
		}
		else {
			tmp = GetNewResult(value, pos_to_check);
		}

		if (consolidated_results->find(curr_check) == consolidated_results->end()) {
			consolidated_results->insert({ curr_check, tmp });
		}
		else {
			consolidated_results->at(curr_check) = tmp;
		}

		if (is_changed) {
			related_var->insert(toInsert);
			is_changed = false;
		}
	}

	if (!related_var->empty()) {
		auto to_remove = related_var->begin();
		curr_check = *to_remove;
		related_var->erase(to_remove);
		ConsolidateResults(curr_check, related_var, consolidated_results, one_user_result_set, two_user_result_set);
	}

	return consolidated_results;
}

STRING_SET_PTR PQLEvaluator::GetAllSet(STRING_PTR output_var_type) {
	PKB* pkb = new PKB();
	DataManager* dm = pkb->GetDataManager();
	STRING_SET_PTR result = new STRING_SET();

	if (DEBUG) {
		cout << "PQLEvaluator - GetAllSet" << endl;
		cout << "output_var_type: " << output_var_type << endl;

	}

	if (output_var_type->compare(TYPE_STMT)) {
		result = ConvertSet(dm->GetAllStatements());
	}
	else if (output_var_type->compare(TYPE_STMT_ASSIGN)) {
		result = ConvertSet(dm->GetAllStatements(STATEMENT_TYPE::assignStatement));
	}
	else if (output_var_type->compare(TYPE_STMT_CALL)) {
		result = ConvertSet(dm->GetAllStatements(STATEMENT_TYPE::callStatement));
	}
	else if (output_var_type->compare(TYPE_STMT_IF)) {
		result = ConvertSet(dm->GetAllStatements(STATEMENT_TYPE::ifStatement));
	}
	else if (output_var_type->compare(TYPE_STMT_WHILE)) {
		result = ConvertSet(dm->GetAllStatements(STATEMENT_TYPE::whileStatement));
	}
	else if (output_var_type->compare(TYPE_STMT_PRINT)) {
		result = ConvertSet(dm->GetAllStatements(STATEMENT_TYPE::printStatement));
	}
	else if (output_var_type->compare(TYPE_STMT_READ)) {
		result = ConvertSet(dm->GetAllStatements(STATEMENT_TYPE::readStatement));
	}
	else if (output_var_type->compare(TYPE_VAR)) {
		result = ConvertSet(dm->GetAllVariables());
	}
	else if (output_var_type->compare(TYPE_PROC)) {
		result = ConvertSet(dm->GetAllProcedures());
	}
	else {
		// error
		if (DEBUG) {
			cout << "PQLEvaluator - GetAllSet: No such relref." << endl;
		}
		return {};
	}

	if (DEBUG) {
		cout << "PQLEvaluator - GetAllSet: Result set's size = " << result->size() << endl;
	}

	return result;
}

STATEMENT_TYPE PQLEvaluator::GetStmtType(STRING_PTR output_var_type) {
	if (output_var_type->compare(TYPE_STMT_ASSIGN)) {
		return assignStatement;
	}
	else if (output_var_type->compare(TYPE_STMT_IF)) {
		return ifStatement;
	}
	else {
		return whileStatement;
	}
}

STRING_SET_PTR PQLEvaluator::GetIntersectResult(STRING_SET_PTR r1, STRINGLIST_SET_PTR r2, INTEGER pos_to_check) {
	STRING_SET_PTR result = new STRING_SET();

	for (STRING_LIST_PTR i : *r2) {
		STRING_PTR val1 = i->at(pos_to_check);
		for (STRING_PTR j : *r1) {
			STRING_PTR val2 = j;
			if (*val1 == *val2) {
				result->insert(j);
			}
		}
	}

	return result;
}

STRING_SET_PTR PQLEvaluator::GetIntersectResult(STRING_SET_PTR existing_val, STRING_SET_PTR new_val) {
	STRING_SET_PTR result = new STRING_SET();

	for (auto i = existing_val->begin(); i != existing_val->end(); i++) {
		if (new_val->find(*i) != new_val->end())
			result->insert(*i);
	}

	return result;
}

STRINGLIST_SET_PTR PQLEvaluator::GetIntersectResult(STRINGLIST_SET_PTR existing_val, STRINGLIST_SET_PTR new_val) {
	STRINGLIST_SET_PTR result = new STRINGLIST_SET();
	
	for (STRING_LIST_PTR i : *existing_val) {
		for (STRING_LIST_PTR j : *new_val) {
			if (*i == *j) {
				result->insert(i);
			}
		}
	}

	return result;
}

STRING_SET_PTR PQLEvaluator::GetNewResult(STRINGLIST_SET_PTR value, INTEGER pos_to_check) {
	STRING_SET_PTR result = new STRING_SET();

	for (STRING_LIST_PTR i : *value) {
		result->insert(i->at(pos_to_check));
	}

	return result;
}

BOOLEAN PQLEvaluator::RemoveIrrelevant(STRING_SET_PTR value, STRING_SET_PTR tmp) {
	BOOLEAN is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		if (tmp->find(*it) == tmp->end()) {
			it = value->erase(it);
		}
		else {
			it++;
		}
	}

	return is_changed;
}

BOOLEAN PQLEvaluator::RemoveIrrelevant(STRINGLIST_SET_PTR value, STRING_SET_PTR tmp, INTEGER pos_to_check) {
	BOOLEAN is_changed = false;

	auto it = value->begin();
	while (it != value->end()) {
		STRING_PTR v = (*it)->at(pos_to_check);
		if (tmp->find(v) == tmp->end()) {
			it = value->erase(it);
		}
		else {
			++it;
		}
	}

	return is_changed;
}

STRING_SET_PTR PQLEvaluator::ConvertSet(INTEGER_SET_PTR result_set) {
	STRING_SET_PTR final_result = new STRING_SET();

	if (!result_set->empty()) {
		for (INTEGER k : *result_set) {
			STRING_PTR value = new STRING(to_string(k));
			final_result->insert(value);
		}
	}
	return final_result;
}

STRING_SET_PTR PQLEvaluator::ConvertSet(STRING_SET_PTR result_set) {
	STRING_SET_PTR final_result = new STRING_SET();
	
	if (!result_set->empty()) {
		for (STRING_PTR k : *result_set) {
			final_result->insert(k);
		}
	}
	return final_result;
}

STRING_SET_PTR PQLEvaluator::ConvertSet(DOUBLE_SET_PTR result_set) {
	STRING_SET_PTR final_result = new STRING_SET();

	if (!result_set->empty()) {
		for (DOUBLE k : *result_set) {
			STRING_PTR value = new STRING(to_string((int) k));
			final_result->insert(value);
		}
	}
	return final_result;
}

STRINGLIST_SET_PTR PQLEvaluator::ConvertSet(STMT_STMT_PAIR_LIST* result_set) {
	STRINGLIST_SET_PTR final_result = new STRINGLIST_SET();

	if (!result_set->empty()) {
		for (STMT_STMT_PAIR* k : *result_set) {
			STRING_PTR first = new STRING(to_string(k->s1));
			STRING_PTR second = new STRING(to_string(k->s2));

			STRING_LIST_PTR value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result->insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET_PTR PQLEvaluator::ConvertSet(STMT_VAR_PAIR_LIST* result_set) {
	STRINGLIST_SET_PTR final_result = new STRINGLIST_SET();

	if (!result_set->empty()) {
		for (STMT_VAR_PAIR* k : *result_set) {
			STRING_PTR first = new STRING(to_string(k->s));
			STRING_PTR second = k->v;

			STRING_LIST_PTR value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result->insert(value);
		}
	}

	return final_result;
}

STRINGLIST_SET_PTR PQLEvaluator::ConvertSet(PROC_VAR_PAIR_LIST* result_set) {
	STRINGLIST_SET_PTR final_result = new STRINGLIST_SET();

	if (!result_set->empty()) {
		for (PROC_VAR_PAIR* k : *result_set) {
			STRING_PTR first = k->p;
			STRING_PTR second = k->v;

			STRING_LIST_PTR value = new STRING_LIST();
			value->push_back(first);
			value->push_back(second);

			final_result->insert(value);
		}
	}

	return final_result;
}


STRING_LIST_PTR PQLEvaluator::IsKey(STRING_LIST_PTR key, STRINGLIST_STRINGLISTSET_MAP_PTR two_user_result_set) {
	for (auto k = two_user_result_set->cbegin(); k != two_user_result_set->cend(); k++) {
		STRING_LIST_PTR existing_key = (*k).first;

		if (*key == *existing_key) {
			return existing_key;
		}
	}

	return nullptr;
}

BOOLEAN PQLEvaluator::IsVar(STRING_PTR var, STRING_STRING_MAP_PTR var_map) {
	if (var_map->find(var) == var_map->end()) {
		return false;
	}
	else {
		return true;
	}
}


BOOLEAN PQLEvaluator::IsWildCard(STRING_PTR var) {
	if (var->compare("_") == 0) {
		return true;
	}
	else {
		return false;
	}
}


INTEGER PQLEvaluator::ParsingStmtRef(STRING_PTR param) {
	if (param->compare("_") == 0) {
		return -1;
	}
	else {
		return stoi(*param);
	}
}

STRING_PTR PQLEvaluator::ParsingEntRef(STRING_PTR param) {
	if (param->compare("_") == 0) {
		return nullptr;
	}
	else {
		return param;
	}
}