#include "PQLEvaluator.h"
#include <iostream>
#include "pkb/PKB.h"

QueryResult PQLEvaluator::evaluate(QueryInfo queryInfo) {
	unordered_map<unordered_set<string>*, unordered_set<string>> interResults = unordered_map<unordered_set<string>*, unordered_set<string>>();
	QueryResult finalResult = QueryResult();	

	unordered_map<string, string> varMap = queryInfo.getVarMap();
	unordered_map<string, vector<vector<string>>> relRefMap = queryInfo.getRelRefMap();
	string outputVar = queryInfo.getOutputVar();
	string outputVarType = varMap.at(outputVar);

	unordered_set<tuple<string, string, string>*> boolSet = unordered_set<tuple<string, string, string>*>();
	unordered_set<tuple<string, string, string>*> getSet = unordered_set<tuple<string, string, string>*>();
	
	// Parse clauses conditions
	for (auto f = relRefMap.cbegin(); f != relRefMap.cend(); f++) {
		string fCall = (*f).first;
		vector<vector<string>> allParams = (*f).second;

		for (vector<string> p : allParams) {
			if (fCall.compare(TYPE_COND_PATTERN) == 0) {
				/*PKB pkb;
				
				unordered_set<string>* key = &unordered_set<string>();
				(*key).insert(p[2]);

				string parsedParam1 = parsingEntRef(p[0], varMap);
				STATEMENT_TYPE type = assignStatement;
				unordered_set<int>* value = pkb.GetPattern(&type, &parsedParam1, &p[1]);
				if (value->empty()) {
					finalResult.setResult({});
					return finalResult;
				}

				unordered_set<string> finalValue = convertSet(*value);

				interResults.insert({ key, finalValue });*/
			}
			else if (outputVarType.compare(TYPE_CONST) == 0) {
				boolSet.insert(&make_tuple(fCall, p[0], p[1]));
			}
			else {
				if (varMap.find(p[0]) == varMap.end() && varMap.find(p[1]) == varMap.end()) {
					// both params != user declared var
					// boolSet
					boolSet.insert(&make_tuple(fCall, p[0], p[1]));
				}
				else {
					// 1 or both param(s) == user declared var
					// getSet
					getSet.insert(&make_tuple(fCall, p[0], p[1]));
				}
			}
		}
	}
	
	// Evaluate boolean-type clauses
	// FALSE -> return empty; ALL TRUE -> continue
	for (tuple<string, string, string>* func : boolSet) {
		if (!evaluateBoolSet(*func)) {
			finalResult.setResult({});
			return finalResult;
		}
	}
	
	
	// Evaluate get-type clauses
	// Store in interResult: KEY = user declared param(s), VALUE = result set
	// Break condition: VALUE = {} implies FALSE -> return empty
	for (tuple<string, string, string>* func : getSet) {
		unordered_set<string>* key = &unordered_set<string>();
		if (varMap.find(get<1>(*func)) != varMap.end()) {
			(*key).insert(get<1>(*func));
		}

		if (varMap.find(get<2>(*func)) != varMap.end()) {
			(*key).insert(get<2>(*func));
		}

		unordered_set<string> value = evaluateGetSet(*func, varMap, outputVarType);

		// Empty result set means the result is FALSE -> return empty
		if (value.empty()) {
			finalResult.setResult({});
			return finalResult;
		}
		
		// Check if KEY exists
		// TRUE -> Perform AND operation on VALUE sets
		// FALSE -> insert KEY-VALUE pair
		if (interResults.find(key) == interResults.end()) {
			interResults.insert({ key , value });
		}
		else {
			interResults.at(key) = getANDResult(interResults.at(key), value);
		}
		
	}
	
	// Check if output variable is CONSTANT: 
	// TRUE -> return output var
	// FALSE -> AND the relevant result sets
	if (outputVarType.compare(TYPE_CONST) == 0) {
		PKB pkb = PKB();
		
		unordered_set<double>* result = pkb.GetDataManager()->GetAllConstants();
		finalResult.setResult(convertSet(result));

		return finalResult;
	}
	else {
		// Include outputVar as KEY in interResult; VALUE = getAll<ENTITY_TYPE of outputVar>
		unordered_set<string> set = unordered_set<string>();
		set.insert(outputVar);
		unordered_set<string>* key = &set;

		unordered_set<string> value = getAllSet(outputVarType);
		if (interResults.find(key) == interResults.end()) {
			interResults.insert({ key, value });
		}
		else {
			interResults.at(key) = getANDResult(interResults.at(key), value);
		}

		// Consolidate results
		unordered_set<string> varToInclude = { outputVar };
		for (auto k = interResults.cbegin(); k != interResults.cend(); k++) {
			unordered_set<string>* key = (*k).first;
			if (isContainingVar(varToInclude, *key)) {

				for (string p: *key) {
					if (varMap.at(p).compare(outputVarType) == 0) {
						varToInclude.insert(p);
					}
				}

				// varToInclude.insert((*k).first.begin(), (*k).first.end());

				if (finalResult.isEmpty()) {
					finalResult.setResult((*k).second);
				}
				else {
					finalResult.setResult(getANDResult((*k).second, finalResult.getResult()));
				}
			}
		}
	}
	
	return finalResult;
}

bool PQLEvaluator::evaluateBoolSet(tuple<string, string, string> func) {
	PKB pkb = PKB();
	RelationManager* rm = pkb.GetRelationManager();

	string fCall = get<0>(func);
	string param1 = get<1>(func);
	string param2 = get<2>(func);

	if (fCall.compare(TYPE_COND_FOLLOWS) == 0) {
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
		return rm->IsStmtUses(parsingStmtRef(param1), &parsingEntRef(param2));
	}
	else if (fCall.compare(TYPE_COND_USES_P) == 0) {
		return rm->IsProcUses(&parsingEntRef(param1), &parsingEntRef(param2));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		return rm->IsStmtModifies(parsingStmtRef(param1), &parsingEntRef(param2));
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_P) == 0) {
		return rm->IsProcModifies(&parsingEntRef(param1), &parsingEntRef(param2));
	}
	else {
		// error
	}
}

int PQLEvaluator::parsingStmtRef(string param) {
	if (param.compare("_") == 0) {
		return 0;
	}
	else {
		return stoi(param);
	}
}

string PQLEvaluator::parsingEntRef(string param) {
	if (param.compare("_") == 0) {
		return "";
	}
	else {
		return param;
	}
}

unordered_set<string> PQLEvaluator::evaluateGetSet(tuple<string, string, string> func, unordered_map<string, string> varMap, string outputVarType) {
	PKB pkb = PKB();
	RelationManager* rm = pkb.GetRelationManager();

	string fCall = get<0>(func);
	string param1 = get<1>(func);
	string param2 = get<2>(func);

	if (fCall.compare(TYPE_COND_FOLLOWS) == 0) {
		int parsedParam1 = parsingStmtRef(param1, varMap);
		int parsedParam2 = parsingStmtRef(param2, varMap);

		//unordered_set<int>* result = pkb->GetFollows(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		//return convertSet(*result);
	}
	else if (fCall.compare(TYPE_COND_FOLLOWS_T) == 0) {
		//unordered_set<int>* result = pkb->GetFollowsStar(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		//return convertSet(*result);
	}
	else if (fCall.compare(TYPE_COND_PARENT) == 0) {
		//unordered_set<int>* result = pkb->GetParent(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		//return convertSet(*result);
	}
	else if (fCall.compare(TYPE_COND_PARENT_T) == 0) {
		//unordered_set<int>* result = pkb->GetParentStar(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		//return convertSet(*result);
	}
	/*else if (fCall.compare(TYPE_COND_USES_S) == 0 || fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		int parsedParam1 = parsingStmtRef(param1, varMap);
		string parsedParam2 = parsingEntRef(param2, varMap);

		if ((isVar(param1, varMap) && isVar(param2, varMap)) || (isVar(param1, varMap) && !isVar(param2, varMap))) {
			// both user declared var
			if (isStmtType(outputVarType)) {
				unordered_set<string> result = convertSet(*(rm->GetAllInverseStmtUses()));
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = rm->GetAllStmtUses();
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param2, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string> result = convertSet(*(rm->GetInverseStmtUses(&parsedParam2)));
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param2, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = rm->GetStmtUses(parsedParam1);
			return convertSet(result);
		}
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_S) == 0) {
		int parsedParam1 = parsingStmtRef(param1, varMap);
		string parsedParam2 = parsingEntRef(param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVarType.compare(TYPE_STMT) == 0 || outputVarType.compare(TYPE_STMT_ASSIGN) == 0 ||
					outputVarType.compare(TYPE_STMT_CALL) == 0 || outputVarType.compare(TYPE_STMT_IF) == 0 ||
					outputVarType.compare(TYPE_STMT_WHILE) == 0 || outputVarType.compare(TYPE_STMT_PRINT) == 0 ||
					outputVarType.compare(TYPE_STMT) == 0) {
				unordered_set<string> result = convertSet(*(rm->GetInverseStmtModifies(&parsedParam2)));
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = rm->GetStmtModifies(parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string> result = convertSet(*(rm->GetInverseStmtModifies(&parsedParam2)));
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = rm->GetStmtModifies(parsedParam1);
			return convertSet(result);
		}
	}
	else if (fCall.compare(TYPE_COND_USES_P) == 0) {
		string parsedParam1 = parsingEntRef(param1, varMap);
		string parsedParam2 = parsingEntRef(param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVarType.compare(TYPE_STMT) == 0 || outputVarType.compare(TYPE_STMT_ASSIGN) == 0 ||
					outputVarType.compare(TYPE_STMT_CALL) == 0 || outputVarType.compare(TYPE_STMT_IF) == 0 ||
					outputVarType.compare(TYPE_STMT_WHILE) == 0 || outputVarType.compare(TYPE_STMT_PRINT) == 0 ||
					outputVarType.compare(TYPE_STMT) == 0) {
				unordered_set<string*> interResult = *(rm->GetInverseProcUses(&parsedParam2));
				unordered_set<string> result = convertSet(&interResult);
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = rm->GetProcUses(&parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string*> interResult = *(rm->GetInverseProcUses(&parsedParam2));
			unordered_set<string> result = convertSet(&interResult);
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = rm->GetProcUses(&parsedParam1);
			return convertSet(result);
		}
	}
	else if (fCall.compare(TYPE_COND_MODIFIES_P) == 0) {
		string parsedParam1 = parsingEntRef(param1, varMap);
		string parsedParam2 = parsingEntRef(param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVarType.compare(TYPE_STMT) == 0 || outputVarType.compare(TYPE_STMT_ASSIGN) == 0 ||
					outputVarType.compare(TYPE_STMT_CALL) == 0 || outputVarType.compare(TYPE_STMT_IF) == 0 ||
					outputVarType.compare(TYPE_STMT_WHILE) == 0 || outputVarType.compare(TYPE_STMT_PRINT) == 0 ||
					outputVarType.compare(TYPE_STMT) == 0) {
				unordered_set<string*> interResult = *(rm->GetInverseProcModifies(&parsedParam2));
				unordered_set<string> result = convertSet(&interResult);
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = rm->GetProcModifies(&parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string*> interResult = *(rm->GetInverseProcModifies(&parsedParam2));
			unordered_set<string> result = convertSet(&interResult);
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = rm->GetProcModifies(&parsedParam1);
			return convertSet(result);
		}
	}*/
	else {
		//error 
	}

	return {};
}

int PQLEvaluator::parsingStmtRef(string param, unordered_map<string, string> varMap) {
	if (param.compare("_") == 0 || varMap.find(param) != varMap.end()) {
		return 0;
	}
	else {
		return stoi(param);
	}
}

string PQLEvaluator::parsingEntRef(string param, unordered_map<string, string> varMap) {
	if (param.compare("_") == 0 || varMap.find(param) != varMap.end()) {
		return "";
	}
	else {
		return param;
	}
}

unordered_set<string> PQLEvaluator::convertSet(unordered_set<int> resultSet) {
	unordered_set<string> finalResult = unordered_set<string>();

	if (!resultSet.empty()) {
		for (int k : resultSet) {
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

unordered_set<string> PQLEvaluator::getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal) {
	unordered_set<string> result = {};
	unordered_set<string> loop = existingVal;
	unordered_set<string> notLoop = newVal;

	if ((int) loop.size() > notLoop.size()) {
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
	}
}

bool PQLEvaluator::isContainingVar(unordered_set<string> relatedVar, unordered_set<string> keys) {
	for (auto i = keys.begin(); i != keys.end(); i++) {
		if (relatedVar.find(*i) != relatedVar.end()) {
			return true;
		}
	}

	return false;
}

bool PQLEvaluator::isVar(string var, unordered_map<string, string> varMap) {
	if (varMap.find(var) == varMap.end()) {
		return false;
	}
	else {
		return true;
	}
}

bool PQLEvaluator::isStmtType(string type) {
	if (type.compare(TYPE_STMT) == 0 || type.compare(TYPE_STMT_ASSIGN) == 0 ||
		type.compare(TYPE_STMT_CALL) == 0 || type.compare(TYPE_STMT_IF) == 0 ||
		type.compare(TYPE_STMT_WHILE) == 0 || type.compare(TYPE_STMT_PRINT) == 0 ||
		type.compare(TYPE_STMT) == 0) {
		return true;
	}
	else {
		return false;
	}
}