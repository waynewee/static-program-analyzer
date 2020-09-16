#include "PQLEvaluator.h"
#include <iostream>
#include "pkb/PKB.h"

QueryResult PQLEvaluator::evaluate(QueryInfo queryInfo) {
	unordered_set<vector<string>*> noUserDeclaredSet = unordered_set<vector<string>*>();
	unordered_set<vector<string>*> oneUserDeclaredSet = unordered_set<vector<string>*>();
	unordered_set<vector<string>*> twoUserDeclaredSet = unordered_set<vector<string>*>();
	unordered_map<string, unordered_set<string>> oneUserResultSet = unordered_map<string, unordered_set<string>>();
	unordered_map<vector<string>*, unordered_set<vector<string>*>> twoUserResultSet = unordered_map<vector<string>*, unordered_set<vector<string>*>>();
	QueryResult finalResult = QueryResult();

	unordered_map<string, string> varMap = queryInfo.getVarMap();
	unordered_map<string, vector<vector<string>>> relRefMap = queryInfo.getRelRefMap();
	string outputVar = queryInfo.getOutputVar();
	string outputVarType = varMap.at(outputVar);

	// Parse clauses conditions
	for (auto f = relRefMap.cbegin(); f != relRefMap.cend(); f++) {
		string fCall = *(new string((*f).first));
		vector<vector<string>> allParams = *(new vector<vector<string>>((*f).second));

		for (vector<string> p : allParams) {
			vector<string>* set = new vector<string>();
			set->push_back(fCall);
			set->push_back(p[0]);
			set->push_back(p[1]);

			if (set->empty()) {
				// error
				cout << "error creating value for sets" << endl;
				finalResult.setResult({});
				return finalResult;
			}

			if (fCall.compare(TYPE_COND_PATTERN) == 0) {
				// PATTERN clause = Special case
				set->push_back(p[2]);
				if (set->size() != 4) {
					// error
					cout << "error creating value for pattern sets" << endl;
					finalResult.setResult({});
					return finalResult;
				}

				if (!isVar(p[0], varMap)) {
					// Pattern clause with assign & var
					if (twoUserDeclaredSet.insert(set).second == 0) {
						// error
						cout << "error inserting twoUserDeclaredSet" << endl;
					}
				}
				else if (isVar(p[0], varMap)) {
					// Pattern clause with assign only
					if (oneUserDeclaredSet.insert(set).second == 0) {
						// error
						cout << "error inserting oneUserDeclaredSet" << endl;
					}
				}
			}
			else {
				if (outputVarType.compare(TYPE_CONST) == 0 || (!isVar(p[0], varMap) && !isVar(p[1], varMap))) {
					// outputVar == CONST type
					// OR
					// both params != user declared var
					if (noUserDeclaredSet.insert(set).second == 0) {
						// error
						cout << "error inserting noUserDeclaredSet" << endl;
					}
				}
				else if ((isVar(p[0], varMap) && !isVar(p[1], varMap)) || (!isVar(p[0], varMap) && isVar(p[1], varMap))) {
					// 1 param == user declared var
					if (oneUserDeclaredSet.insert(set).second == 0) {
						// error
						cout << "error inserting oneUserDeclaredSet" << endl;
					}
				}
				else if (isVar(p[0], varMap) && isVar(p[1], varMap)) {
					// 2 params = user declared var
					if (twoUserDeclaredSet.insert(set).second == 0) {
						// error
						cout << "error inserting twoUserDeclaredSet" << endl;
					}
				}
				else {
					// error
					cout << "params invalid at parsing clause conditions" << endl;
					finalResult.setResult({});
					return finalResult;
				}
			}
		}
	}

	// Evaluate noUserDeclaredSet -> T/F clauses
	// FALSE -> return empty; ALL TRUE -> continue
	for (const vector<string>* func : noUserDeclaredSet) {
		if (!evaluateNoUserDeclaredSet(*func)) {
			finalResult.setResult({});
			return finalResult;
		}
	}

	// Check if outputVar is CONST type
	// TRUE -> return all const vals; FALSE -> continue
	if (outputVarType.compare(TYPE_CONST) == 0) {
		PKB pkb = PKB();

		unordered_set<double>* result = pkb.GetDataManager()->GetAllConstants();
		finalResult.setResult(convertSet(result));

		return finalResult;
	}

	// Evaluate oneUserDeclaredSet
	// EMPTY set -> return empty;
	for (const vector<string>* func : oneUserDeclaredSet) {
		string key = string();
		unordered_set<string> value = unordered_set<string>();

		string fCall = (*func)[0];
		string param1 = (*func)[1];
		string param2 = (*func)[2];

		if (fCall.compare(TYPE_COND_PATTERN) == 0) {
			/*string param3 = (*func)[3];
			key.assign(param3);

			PKB pkb = PKB();
			STATEMENT_TYPE stmtType = getStmtType(outputVarType);
			value = pkb.GetRelationManager().GetPattern(stmtType, parsingEntRef(param1), param2);*/
			cout << "pattern not implemented" << endl;
		} else if (isVar(param1, varMap)) {
			// getInverseXXX() branch
			key.assign(param1);

			// value = evaluateOneDeclaredSet(fCall, param2);
			value = evaluateInverseOneDeclaredSet(fCall, param2);
		}
		else if (isVar(param2, varMap)) {
			// getXXX() branch
			key.assign(param2);

			value = evaluateOneDeclaredSet(fCall, param1);
			// value = evaluateInverseOneDeclaredSet(fCall, param1);
		}
		else {
			// error
			cout << "no user declared var in oneUserDeclaredSet loop" << endl;
			finalResult.setResult({});
			return finalResult;
		}

		if (value.empty()) {
			cout << "empty value set at oneUserDeclaredSet" << endl;
			finalResult.setResult({});
			return finalResult;
		}

		// Check if KEY exists
		// TRUE -> AND the results; FALSE -> insert the results
		if (oneUserResultSet.find(key) == oneUserResultSet.end()) {
			
			oneUserResultSet.insert({ key, value });
		}
		else {
			oneUserResultSet.at(key) = getANDResult(oneUserResultSet.at(key), value);
		}
	}

	// Evaluate twoUserDeclaredSet
	// EMPTY set -> return empty 
	for (const vector<string>* func : twoUserDeclaredSet) {
		vector<string>* key = &vector<string>();
		unordered_set<vector<string>*> value = unordered_set<vector<string>*>();

		string fCall = (*func)[0];
		string param1 = (*func)[1];
		string param2 = (*func)[2];

		(*key).push_back(param1);
		(*key).push_back(param2);

		if (fCall.compare(TYPE_COND_PATTERN) == 0) {
			/*string param3 = (*func)[3];
			(*key).push_back(param3);
			(*key).push_back(param1);

			PKB pkb = PKB();
			STATEMENT_TYPE stmtType = getStmtType(outputVarType);
			value = pkb.GetRelationManager().GetPattern(stmtType, nullptr, param2);*/
			cout << "pattern not implemented" << endl;
		}
		else {
			value = evaluateTwoDeclaredSet(fCall);
		}

		if (value.empty()) {
			cout << "empty value set at twoUserDeclaredSet" << endl;
			finalResult.setResult({});
			return finalResult;
		}

		// Check if KEY exists
		// TRUE -> AND the results; FALSE -> insert the results
		vector<string>* checkKey = isKey(*key, twoUserResultSet);
		if (checkKey == nullptr) {
			twoUserResultSet.insert({ key, value });
		}
		else {
			twoUserResultSet.at(checkKey) = getANDResult(twoUserResultSet.at(checkKey), value);
		}
	}
	
	// Consolidate results
	unordered_map<string, unordered_set<string>> consolidatedResults = unordered_map<string, unordered_set<string>>();
	unordered_set<string> relatedVar = unordered_set<string>();
	consolidatedResults = consolidateResults(outputVar, relatedVar, consolidatedResults, oneUserResultSet, twoUserResultSet);

	// Check if outputVar is in consolidatedResults
	// YES -> return corresponding values; NO -> getALLXXX(outputVarType)
	if (consolidatedResults.find(outputVar) != consolidatedResults.end()) {
		cout << "output var found in consolidatedResults" << endl;
		finalResult.setResult(consolidatedResults.at(outputVar));
	}
	else {
		unordered_set<string> value = unordered_set<string>();
		value = getAllSet(outputVarType);
		finalResult.setResult(value);
	}


	// Return result as QueryResult
	cout << finalResult.getResult().size() << endl;
	for (string const s : finalResult.getResult()) {
		cout << s << endl;
	}
	
	return finalResult;
}

bool PQLEvaluator::evaluateNoUserDeclaredSet(vector<string> func) {
	PKB pkb = PKB();
	RelationManager* rm = pkb.GetRelationManager();

	string fCall = func[0];
	string param1 = func[1];
	string param2 = func[2];

	cout << "in evaluateNoUserDeclaredSet" << endl;
	cout << "fcall: " << fCall << "; param1: " << param1 << "; param2: " << param2 << endl;

	if (fCall.compare(TYPE_COND_FOLLOWS) == 0) {
		cout << "reached follows" << endl;
		return rm->IsFollows(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall.compare(TYPE_COND_FOLLOWS_T) == 0) {
		return rm->IsFollowsStar(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall.compare(TYPE_COND_PARENT) == 0) {
		return rm->IsParent(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall.compare(TYPE_COND_PARENT_T) == 0) {
		return rm->IsParentStar(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall.compare(TYPE_COND_USES_S) == 0) {
		return rm->IsStmtUses(parsingStmtRef(param1), parsingEntRef(param2));
	}
	else if (fCall.compare(TYPE_COND_USES_P) == 0) {
		return rm->IsProcUses(parsingEntRef(param1), parsingEntRef(param2));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		return rm->IsStmtModifies(parsingStmtRef(param1), parsingEntRef(param2));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_P) == 0) {
		return rm->IsProcModifies(parsingEntRef(param1), parsingEntRef(param2));
	}
	else {
		// error
		cout << "no such relref" << endl;
		return false;
	}
}

unordered_set<string> PQLEvaluator::evaluateOneDeclaredSet(string fCall, string param) {
	PKB pkb;
	RelationManager* rm = pkb.GetRelationManager();

	cout << "in evaluateOneDeclaredSet" << endl;
	cout << "fcall: " << fCall << "; param: " << param << endl;

	if (fCall.compare(TYPE_COND_FOLLOWS) == 0) {
		cout << "reached follows" << endl;
		// int parsedParam = parsingStmtRef(param);
		// cout << parsedParam << endl;
		unordered_set<int> result = *(rm->GetFollows(parsingStmtRef(param)));
		// cout << "RESULT SIZE: " << result.size() << endl;
		return convertSet(result);
	}
	else if (fCall.compare(TYPE_COND_FOLLOWS_T) == 0) {
		return convertSet(*(rm->GetFollowsStars(parsingStmtRef(param))));
	}
	else if (fCall.compare(TYPE_COND_PARENT) == 0) {
		return convertSet(*(rm->GetParents(parsingStmtRef(param))));
	}
	else if (fCall.compare(TYPE_COND_PARENT_T) == 0) {
		return convertSet(*(rm->GetParentStars(parsingStmtRef(param))));
	}
	else if (fCall.compare(TYPE_COND_USES_S) == 0) {
		return convertSet(rm->GetStmtUses(parsingStmtRef(param)));
	}
	else if (fCall.compare(TYPE_COND_USES_P) == 0) {
		return convertSet(rm->GetProcUses(parsingEntRef(param)));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		return convertSet(rm->GetStmtModifies(parsingStmtRef(param)));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_P) == 0) {
		return convertSet(rm->GetProcModifies(parsingEntRef(param)));
	}
	else {
		// error
		cout << "no such relref" << endl;
		return {};
	}
}

unordered_set<string> PQLEvaluator::evaluateInverseOneDeclaredSet(string fCall, string param) {
	PKB* pkb = new PKB();
	RelationManager* rm = pkb->GetRelationManager();

	cout << "in evaluateInverseOneDeclaredSet" << endl;
	cout << "fcall: " << fCall << "; param: " << param << endl;

	if (fCall.compare(TYPE_COND_FOLLOWS) == 0) {
		cout << "reached follows" << endl;
		// int parsedParam = parsingStmtRef(param);
		// cout << parsedParam << endl;
		unordered_set<int> result = *(rm->GetInverseFollows(parsingStmtRef(param)));
		// cout << "RESULT SIZE: " << result.size() << endl;
		return convertSet(result);
	}
	else if (fCall.compare(TYPE_COND_FOLLOWS_T) == 0) {
		return convertSet(*(rm->GetInverseFollowsStars(parsingStmtRef(param))));
	}
	else if (fCall.compare(TYPE_COND_PARENT) == 0) {
		return convertSet(*(rm->GetInverseParents(parsingStmtRef(param))));
	}
	else if (fCall.compare(TYPE_COND_PARENT_T) == 0) {
		return convertSet(*(rm->GetInverseParentStars(parsingStmtRef(param))));
	}
	else if (fCall.compare(TYPE_COND_USES_S) == 0) {
		return convertSet(*(rm->GetInverseStmtUses(parsingEntRef(param))));
	}
	else if (fCall.compare(TYPE_COND_USES_P) == 0) {
		return convertSet(rm->GetInverseProcUses(parsingEntRef(param)));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		return convertSet(*(rm->GetInverseStmtModifies(parsingEntRef(param))));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_P) == 0) {
		return convertSet(rm->GetInverseProcModifies(parsingEntRef(param)));
	}
	else {
		// error
		cout << "no such relref" << endl;
		return {};
	}
}

unordered_set<vector<string>*> PQLEvaluator::evaluateTwoDeclaredSet(string fCall) {
	PKB pkb = PKB();
	RelationManager* rm = pkb.GetRelationManager();

	cout << "in evaluateTwoDeclaredSet" << endl;
	cout << "fcall: " << fCall << endl;

	if (fCall.compare(TYPE_COND_FOLLOWS) == 0) {
		cout << "reached Follows" << endl;
		return convertSet(*(rm->GetAllFollows()));
	}
	else if (fCall.compare(TYPE_COND_FOLLOWS_T) == 0) {
		cout << "reached Follows*" << endl;
		return convertSet(*(rm->GetAllFollowsStars()));
	}
	else if (fCall.compare(TYPE_COND_PARENT) == 0) {
		cout << "reached Parent" << endl;
		return convertSet(*(rm->GetAllParents()));
	}
	else if (fCall.compare(TYPE_COND_PARENT_T) == 0) {
		cout << "reached Parent*" << endl;
		return convertSet(*(rm->GetAllParentStars()));
	}
	else if (fCall.compare(TYPE_COND_USES_S) == 0) {
		cout << "reached UsesS" << endl;
		return convertSet(*rm->GetAllStmtUses());
	}
	else if (fCall.compare(TYPE_COND_USES_P) == 0) {
		cout << "reached UsesP" << endl;
		return convertSet(*(rm->GetAllProcUses()));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		cout << "reached ModifiesS" << endl;
		return convertSet(*(rm->GetAllStmtModifies()));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_P) == 0) {
		cout << "reached ModifiesP" << endl;
		return convertSet(*(rm->GetAllProcModifies()));
	}
	else {
		// error
		cout << "no such relref" << endl;
		return {};
	}
}

unordered_map<string, unordered_set<string>> PQLEvaluator::consolidateResults(string currCheck, unordered_set<string> relatedVar, 
		unordered_map<string, unordered_set<string>> consolidatedResults, unordered_map<string, unordered_set<string>> oneUserResultSet, 
		unordered_map<vector<string>*, unordered_set<vector<string>*>> twoUserResultSet) {

	bool isChanged = false;

	for (auto r2 = twoUserResultSet.cbegin(); r2 != twoUserResultSet.cend(); r2++) {
		vector<string> key = *((*r2).first);
		unordered_set<vector<string>*> value = (*r2).second;
		string toInsert = string();
		int positionToCheck = int();

		// Check if KEY contains the entity being checked
		// TRUE -> add its other entity into (PENDING) related var 
		// FALSE -> go next loop
		if (key[0].compare(currCheck)) {
			toInsert.append(key[1]);
			positionToCheck = 0;
		}
		else if (key[1].compare(currCheck)) {
			toInsert.append(key[0]);
			positionToCheck = 1;
		}
		else {
			continue;
		}

		// Check if oneUserResultSet contains entity being checked
		// TRUE -> AND the results
		// FALSE -> create new entry and add all the values 
		// BOTH -> insert results into consolidatedResults
		unordered_set<string> tmp = unordered_set<string>();
		if (oneUserResultSet.find(currCheck) != oneUserResultSet.end()) {
			unordered_set<string> r1 = oneUserResultSet.at(currCheck);
			tmp = getANDResult(r1, value, positionToCheck);

			// Remove irrelevant data values from both sets
			// If either removes data, add (PENDING) related var into relatedVar
			if (removeIrrelevant(&value, tmp, positionToCheck) || removeIrrelevant(&r1, tmp)) {
				isChanged = true;
			}
		}
		else {
			tmp = getNewResult(value, positionToCheck);
		}

		if (consolidatedResults.find(currCheck) == consolidatedResults.end()) {
			consolidatedResults.at(currCheck) = tmp;
		}
		else {
			consolidatedResults.insert({ currCheck, tmp });
		}

		if (isChanged) {
			relatedVar.insert(toInsert);
			isChanged = false;
		}
	}

	if (!relatedVar.empty()) {
		auto toRemove = relatedVar.begin();
		currCheck = *toRemove;
		relatedVar.erase(toRemove);
		cout << "removed related var = " << currCheck << endl;

		consolidateResults(currCheck, relatedVar, consolidatedResults, oneUserResultSet, twoUserResultSet);
	}

	return consolidatedResults;
}

unordered_set<string> PQLEvaluator::getAllSet(string outputVarType) {
	PKB pkb = PKB();
	DataManager* dm = pkb.GetDataManager();

	cout << "in getAllSet" << endl;
	cout << "outputVarType: " << outputVarType << endl;

	if (outputVarType == TYPE_STMT) {
		unordered_set<int> result = *(dm->GetAllStatements());
		return convertSet(result);
	}
	else if (outputVarType == TYPE_STMT_ASSIGN) {
		unordered_set<int> result = *(dm->GetAllStatements(STATEMENT_TYPE::assignStatement));
		return convertSet(result);
	}
	else if (outputVarType == TYPE_STMT_CALL) {
		unordered_set<int> result = *(dm->GetAllStatements(STATEMENT_TYPE::callStatement));
		return convertSet(result);
	}
	else if (outputVarType == TYPE_STMT_IF) {
		unordered_set<int> result = *(dm->GetAllStatements(STATEMENT_TYPE::ifStatement));
		return convertSet(result);
	}
	else if (outputVarType == TYPE_STMT_WHILE) {
		unordered_set<int> result = *(dm->GetAllStatements(STATEMENT_TYPE::whileStatement));
		return convertSet(result);
	}
	else if (outputVarType == TYPE_STMT_PRINT) {
		unordered_set<int> result = *(dm->GetAllStatements(STATEMENT_TYPE::printStatement));
		return convertSet(result);
	}
	else if (outputVarType == TYPE_STMT_READ) {
		unordered_set<int> result = *(dm->GetAllStatements(STATEMENT_TYPE::readStatement));
		return convertSet(result);
	}
	else if (outputVarType == TYPE_VAR) {
		unordered_set<string*>* result = dm->GetAllVariables();
		return convertSet(result);
	}
	else if (outputVarType == TYPE_PROC) {
		unordered_set<string*>* result = dm->GetAllProcedures();
		return convertSet(result);
	}
	else {
		// error
		cout << "no such relref" << endl;
		return {};
	}
}

STATEMENT_TYPE PQLEvaluator::getStmtType(string outputVarType) {
	if (outputVarType.compare(TYPE_STMT_ASSIGN)) {
		return assignStatement;
	}
	else if (outputVarType.compare(TYPE_STMT_IF)) {
		return ifStatement;
	}
	else {
		return whileStatement;
	}
}

unordered_set<string> PQLEvaluator::getANDResult(unordered_set<string> r1, unordered_set<vector<string>*> r2, int positionToCheck) {
	unordered_set<string> result = *(new unordered_set<string>());

	for (vector<string>* i : r2) {
		for (string j : r1) {
			if ((*i).at(positionToCheck) == j) {
				result.insert(j);
			}
		}
	}

	return result;
}

unordered_set<string> PQLEvaluator::getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal) {

	cout << "in getandresult" << endl;

	unordered_set<string> result = *(new unordered_set<string>());
	unordered_set<string> loop = existingVal;
	unordered_set<string> notLoop = newVal;

	if (loop.size() > notLoop.size()) {
		loop = newVal;
		notLoop = existingVal;
	}

	for (auto i = loop.begin(); i != loop.end(); i++) {
		if (notLoop.find(*i) != notLoop.end())
			result.insert(*i);
	}

	return result;
}

unordered_set<vector<string>*> PQLEvaluator::getANDResult(unordered_set<vector<string>*> existingVal, unordered_set<vector<string>*> newVal) {
	unordered_set<vector<string>*> result = *(new unordered_set<vector<string>*>());
	
	unordered_set<vector<string>*> loop = existingVal;
	unordered_set<vector<string>*> innerLoop = newVal;

	if (loop.size() > innerLoop.size()) {
		loop = newVal;
		innerLoop = existingVal;
	}

	vector<string>* interResult = new vector<string>();
	for (vector<string>* i : loop) {
		for ( vector<string>* j : innerLoop) {
			if (*i == *j) {
				result.insert(i);
			}
		}
	}

	return result;
}

unordered_set<string> PQLEvaluator::getNewResult(unordered_set<vector<string>*> value, int positionToCheck) {
	unordered_set<string> result = *(new unordered_set<string>());

	for (vector<string>* i : value) {
		result.insert((*i).at(positionToCheck));
	}

	return result;
}


bool PQLEvaluator::removeIrrelevant(unordered_set<string>* value, unordered_set<string> tmp) {
	bool isChanged = false;

	auto it = value->begin();
	while (it != value->end()) {
		if (tmp.find(*it) == tmp.end()) {
			it = value->erase(it);
		}
		else {
			++it;
		}
	}

	return isChanged;
}

bool PQLEvaluator::removeIrrelevant(unordered_set<vector<string>*>* value, unordered_set<string> tmp, int positionToCheck) {
	bool isChanged = false;

	auto it = value->begin();
	while (it != value->end()) {
		string v = (*it)->at(positionToCheck);
		if (tmp.find(v) == tmp.end()) {
			it = value->erase(it);
		}
		else {
			++it;
		}
	}

	return isChanged;
}


unordered_set<string> PQLEvaluator::convertSet(unordered_set<int> resultSet) {
	unordered_set<string> finalResult = unordered_set<string>();
	cout << resultSet.size() << endl;
	if (!resultSet.empty()) {
		for (int k : resultSet) {
			cout << k << endl;
			finalResult.insert(to_string(k));
		}
	}
	return finalResult;
}

unordered_set<string> PQLEvaluator::convertSet(unordered_set<string*>* resultSet) {
	unordered_set<string> finalResult = unordered_set<string>();
	if (!(*resultSet).empty()) {
		for (string* k : *resultSet) {
			finalResult.insert(*k);
		}
	}
	return finalResult;
}

unordered_set<string> PQLEvaluator::convertSet(unordered_set<double>* resultSet) {
	unordered_set<string> finalResult = unordered_set<string>();
	if (!(*resultSet).empty()) {
		for (double k : *resultSet) {
			finalResult.insert(to_string(((int)(k))));
		}
	}
	return finalResult;
}

unordered_set<vector<string>*> PQLEvaluator::convertSet(STMT_STMT_PAIR_SET resultSet) {

	unordered_set<vector<string>*> finalResult = *(new unordered_set<vector<string>*>());

	if (!resultSet.empty()) {
		for (STMT_STMT_PAIR* k : resultSet) {
			string first = to_string(k->s1);
			string second = to_string(k->s2);

			vector<string>* value = new vector<string>();
			value->push_back(first);
			value->push_back(second);

			finalResult.insert(value);
		}
	}

	return finalResult;
}

unordered_set<vector<string>*> PQLEvaluator::convertSet(STMT_VAR_PAIR_SET resultSet) {
	unordered_set<vector<string>*> finalResult = *(new unordered_set<vector<string>*>());

	if (!resultSet.empty()) {
		for (STMT_VAR_PAIR* k : resultSet) {
			string first = to_string(k->s);
			string second = *(k->v);

			vector<string>* value = new vector<string>();
			value->push_back(first);
			value->push_back(second);

			finalResult.insert(value);
		}
	}

	return finalResult;
}

unordered_set<vector<string>*> PQLEvaluator::convertSet(PROC_VAR_PAIR_SET resultSet) {
	unordered_set<vector<string>*> finalResult = *(new unordered_set<vector<string>*>());

	if (!resultSet.empty()) {
		for (PROC_VAR_PAIR* k : resultSet) {
			string first = *(k->p);
			string second = *(k->v);

			vector<string>* value = new vector<string>();
			value->push_back(first);
			value->push_back(second);

			finalResult.insert(value);
		}
	}

	return finalResult;
}


vector<string>* PQLEvaluator::isKey(vector<string> key, unordered_map<vector<string>*, unordered_set<vector<string>*>> twoUserResultSet) {
	cout << "in isKey check" << endl;

	for (auto k = twoUserResultSet.cbegin(); k != twoUserResultSet.cend(); k++) {
		vector<string>* existingKey = (*k).first;

		if (key == *existingKey) {
			return existingKey;
		}
	}

	return nullptr;
}

bool PQLEvaluator::isVar(string var, unordered_map<string, string> varMap) {
	if (varMap.find(var) == varMap.end()) {
		return false;
	}
	else {
		return true;
	}
}

bool PQLEvaluator::isWildCard(string var) {
	if (var.compare("_") == 0) {
		return true;
	}
	else {
		return false;
	}
}


int PQLEvaluator::parsingStmtRef(string param) {
	if (param.compare("_") == 0) {
		return -1;
	}
	else {
		return stoi(param);
	}
}

string* PQLEvaluator::parsingEntRef(string param) {
	if (param.compare("_") == 0) {
		return nullptr;
	}
	else {
		return &param;
	}
}