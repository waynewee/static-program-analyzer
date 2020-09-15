#include "PQLEvaluator.h"
#include <iostream>
#include "pkb/PKB.h"

QueryResult PQLEvaluator::evaluate(QueryInfo queryInfo) {
	unordered_set<vector<string>*> noUserDeclaredSet = unordered_set<vector<string>*>();
	unordered_set<vector<string>*> oneUserDeclaredSet = unordered_set<vector<string>*>();
	unordered_set<vector<string>*> twoUserDeclaredSet = unordered_set<vector<string>*>();
	unordered_map<string, unordered_set<string>> oneUserResultSet = unordered_map<string, unordered_set<string>>();
	unordered_map<vector<string>*, unordered_set<string>> twoUserResultSet = unordered_map<vector<string>*, unordered_set<string>>();
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

			if (fCall.compare(TYPE_COND_PATTERN) == 0) {
				// NOT IMPLEMENTED YET.
				cout << "pattern not implemented yet" << endl;
				finalResult.setResult({});
				return finalResult;
			}
			else {
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

		// unordered_set<double>* result = pkb.GetDataManager()->GetAllConstants();
		// finalResult.setResult(convertSet(result));

		// dummy data
		finalResult.setResult({ "c1", "c2", "c3" });

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
		
		if (isVar(param1, varMap)) {
			// getXXX() branch
			key.assign(param1);

			value = evaluateOneDeclaredSet(fCall, param2);
			value = evaluateInverseOneDeclaredSet(fCall, param2);
		}
		else if (isVar(param2, varMap)) {
			// getInverseXXX() branch
			key.assign(param2);

			value = evaluateOneDeclaredSet(fCall, param1);
			value = evaluateInverseOneDeclaredSet(fCall, param1);
		}
		else {
			// error
			cout << "no user declared var in oneUserDeclaredSet loop" << endl;
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

	/*
	// Evaluate twoUserDeclaredSet
	// EMPTY set -> return empty 
	for (const vector<string>* func : twoUserDeclaredSet) {
		vector<string>* key = &vector<string>();
		unordered_set<string> value = unordered_set<string>();

		string fCall = (*func)[0];
		string param1 = (*func)[1];
		string param2 = (*func)[2];

		(*key).push_back(param1);
		(*key).push_back(param2);

		value = evaluateTwoDeclaredSet(fCall);

		// Check if KEY exists
		// TRUE -> AND the results; FALSE -> insert the results
		string* isKey = hasKey(*key, twoUserResultSet);
		if (isKey == nullptr) {
			twoUserResultSet.insert({ key, value });
		}
		else {
			twoUserResultSet.at(key) = getANDResult(twoUserResultSet.at(key), value);
		}
	}
	*/


	// Check if outputVar is in interResults
	// YES -> return corresponding values; NO -> getALLXXX(outputVarType)
	if (oneUserResultSet.find(outputVar) != oneUserResultSet.end()) {
		cout << "Reached" << endl;
		finalResult.setResult(oneUserResultSet.at(outputVar));
	}
	else {
		unordered_set<string> value = unordered_set<string>();
		value = getAllSet(outputVarType);
		finalResult.setResult(value);
	}

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
/*
unordered_set<string> PQLEvaluator::evaluateTwoDeclaredSet(string fCall) {
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
		return convertSet(*rm->GetAllStmtUses()));
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
*/

unordered_set<string> PQLEvaluator::getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal) {

	cout << "in getandresult" << endl;

	unordered_set<string> result = {};
	unordered_set<string> loop = existingVal;
	unordered_set<string> notLoop = newVal;

	if ((int)loop.size() > notLoop.size()) {
		loop = newVal;
		notLoop = existingVal;
	}

	for (auto i = loop.begin(); i != loop.end(); i++) {
		if (notLoop.find(*i) != notLoop.end())
			result.insert(*i);
	}

	return result;
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
/*
unordered_set<string> PQLEvaluator::convertSet(STMT_STMT_PAIR_SET resultSet) {

}

unordered_set<string> PQLEvaluator::convertSet(STMT_VAR_PAIR_SET resultSet) {

}

unordered_set<string> PQLEvaluator::convertSet(PROC_VAR_PAIR_SET resultSet) {

}

string* PQLEvaluator::isKey(vector<string> key, unordered_map<vector<string>*, unordered_set<string>> twoUserResultSet) {
	cout << "in hasKey check" << endl;

	for (auto k = twoUserResultSet.cbegin(); k != twoUserResultSet.cend(); k++) {
		vector<string> existingKey = *((*k).first);

		if (key[0] == existingKey[0] || key[0] == existingKey[1]) {
			return &key[1];
		}

		if (key[1] == existingKey[0] || key[1] == existingKey[1]) {
			return &key[0];
		}
	}

	return nullptr;
}
*/