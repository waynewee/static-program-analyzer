#include "PQLEvaluator.h"

#include "pkb/PKB.h"

QueryResult PQLEvaluator::evaluate(QueryInfo queryInfo) {
	
	unordered_map<unordered_set<string>*, unordered_set<string>> interResults = unordered_map<unordered_set<string>*, unordered_set<string>>();
	QueryResult finalResult = QueryResult();	
	
	unordered_map<string, ENTITY> varMap = queryInfo.getVarMap();
	unordered_map<RELREF, vector<vector<string>>> relRefMap = queryInfo.getRelRefMap();
	string outputVar = queryInfo.getOutputVar();
	ENTITY outputVarEntity = varMap.at(outputVar);
	
	unordered_set<tuple<RELREF, string, string>*> boolSet = unordered_set<tuple<RELREF, string, string>*>();
	unordered_set<tuple<RELREF, string, string>*> getSet = unordered_set<tuple<RELREF, string, string>*>();
	
	// Parse clauses conditions
	for (auto f = relRefMap.cbegin(); f != relRefMap.cend(); f++) {
		RELREF fCall = (*f).first;
		vector<vector<string>> allParams = (*f).second;

		for (vector<string> p : allParams) {
			if (fCall == RELREF::pattern) {
				PKB pkb;

				unordered_set<string>* key = &unordered_set<string>();
				(*key).insert(p[2]);

				string* parsedParam1 = parsingEntRef(&p[0], varMap);
				STATEMENT_TYPE patternType = ((STATEMENT*)&varMap.at(p[2]))->statement_type;
				unordered_set<string> value = convertSet(pkb.GetPattern(&patternType, parsedParam1, &p[1]));

				if (value.empty()) {
					finalResult.setResult({});
					return finalResult;
				}

				interResults.insert({ key, value });
			}
			else if (outputVarEntity.type == ENTITY_TYPE::constant) {
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
	for (tuple<RELREF, string, string>* func : boolSet) {
		if (!evaluateBoolSet(*func)) {
			finalResult.setResult({});
			return finalResult;
		}
	}
	

	// Evaluate get-type clauses
	// Store in interResult: KEY = user declared param(s), VALUE = result set
	// Break condition: VALUE = {} implies FALSE -> return empty
	for (tuple<RELREF, string, string>* func : getSet) {
		unordered_set<string>* key = &unordered_set<string>();
		if (varMap.find(get<1>(*func)) != varMap.end()) {
			(*key).insert(get<1>(*func));
		}

		if (varMap.find(get<2>(*func)) != varMap.end()) {
			(*key).insert(get<2>(*func));
		}

		unordered_set<string> value = evaluateGetSet(*func, varMap, outputVarEntity);

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
	if (outputVarEntity.type == ENTITY_TYPE::constant) {
		finalResult.setResult({ outputVar });
		return finalResult;
	}
	else {
		// Include outputVar as KEY in interResult; VALUE = getAll<ENTITY_TYPE of outputVar>
		unordered_set<string>* key = &unordered_set<string>();
		(*key).insert(outputVar);

		unordered_set<string> value = getAllSet(outputVarEntity);
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
					if (varMap.at(p).type == outputVarEntity.type) {
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

bool PQLEvaluator::evaluateBoolSet(tuple<RELREF, string, string> func) {
	PKB pkb;

	RELREF fCall = get<0>(func);
	string param1 = get<1>(func);
	string param2 = get<2>(func);

	if (fCall == RELREF::Follows) {
		return pkb.IsFollows(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall == RELREF::FollowsT) {
		return pkb.IsFollowsStar(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall == RELREF::Parent) {
		return pkb.IsParent(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall == RELREF::ParentT) {
		return pkb.IsParentStar(parsingStmtRef(param1), parsingStmtRef(param2));
	}
	else if (fCall == RELREF::UsesS) {
		return pkb.IsStmtUses(parsingStmtRef(param1), parsingEntRef(&param2));
	}
	else if (fCall == RELREF::UsesP) {
		return pkb.IsProcUses(parsingEntRef(&param1), parsingEntRef(&param2));
	}
	else if (fCall == RELREF::ModifiesS) {
		return pkb.IsStmtModifies(parsingStmtRef(param1), parsingEntRef(&param2));
	}
	else if (fCall == RELREF::ModifiesP) {
		return pkb.IsProcModifies(parsingEntRef(&param1), parsingEntRef(&param2));
	}
	else {
		// error
	}
}

int PQLEvaluator::parsingStmtRef(string param) {
	if (param.compare("_")) {
		return 0;
	}
	else {
		return stoi(param);
	}
}

string* PQLEvaluator::parsingEntRef(string* param) {
	if ((*param).compare("_")) {
		return NULL;
	}
	else {
		return param;
	}
}

unordered_set<string> PQLEvaluator::evaluateGetSet(tuple<RELREF, string, string> func, unordered_map<string, ENTITY> varMap, ENTITY outputVar) {
	PKB pkb;

	RELREF fCall = get<0>(func);
	string param1 = get<1>(func);
	string param2 = get<2>(func);

	if (fCall == RELREF::Follows) {
		unordered_set<int>* result = pkb.GetFollows(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		return convertSet(result);
	}
	else if (fCall == RELREF::FollowsT) {
		unordered_set<int>* result = pkb.GetFollowsStar(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		return convertSet(result);
	}
	else if (fCall == RELREF::Parent) {
		unordered_set<int>* result = pkb.GetParent(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		return convertSet(result);
	}
	else if (fCall == RELREF::ParentT) {
		unordered_set<int>* result = pkb.GetParentStar(parsingStmtRef(param1, varMap), parsingStmtRef(param2, varMap));
		return convertSet(result);
	}
	else if (fCall == RELREF::UsesS) {
		int parsedParam1 = parsingStmtRef(param1, varMap);
		string* parsedParam2 = parsingEntRef(&param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVar.type == ENTITY_TYPE::statement) {
				unordered_set<string> result = convertSet(pkb.GetInverseStmtUses(parsedParam2));
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = pkb.GetStmtUses(parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string> result = convertSet(pkb.GetInverseStmtUses(parsedParam2));
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = pkb.GetStmtUses(parsedParam1);
			return convertSet(result);
		}
	}
	else if (fCall == RELREF::ModifiesS) {
		int parsedParam1 = parsingStmtRef(param1, varMap);
		string* parsedParam2 = parsingEntRef(&param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVar.type == ENTITY_TYPE::statement) {
				unordered_set<string> result = convertSet(pkb.GetInverseStmtModifies(parsedParam2));
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = pkb.GetStmtModifies(parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string> result = convertSet(pkb.GetInverseStmtModifies(parsedParam2));
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = pkb.GetStmtModifies(parsedParam1);
			return convertSet(result);
		}
	}
	else if (fCall == RELREF::UsesP) {
		string* parsedParam1 = parsingEntRef(&param1, varMap);
		string* parsedParam2 = parsingEntRef(&param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVar.type == ENTITY_TYPE::statement) {
				unordered_set<string> result = convertSet(pkb.GetInverseProcUses(parsedParam2));
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = pkb.GetProcUses(parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string> result = convertSet(pkb.GetInverseProcUses(parsedParam2));
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = pkb.GetProcUses(parsedParam1);
			return convertSet(result);
		}
	}
	else if (fCall == RELREF::ModifiesP) {
		string* parsedParam1 = parsingEntRef(&param1, varMap);
		string* parsedParam2 = parsingEntRef(&param2, varMap);

		if (isVar(param1, varMap) && isVar(param1, varMap)) {
			// both user declared var
			if (outputVar.type == ENTITY_TYPE::statement) {
				unordered_set<string> result = convertSet(pkb.GetInverseProcModifies(parsedParam2));
				return getANDResult(result, getAllSet(varMap.at(param1)));
			}
			else {
				unordered_set<string*>* result = pkb.GetProcModifies(parsedParam1);
				return convertSet(result);
			}
		}
		else if (isVar(param1, varMap) && !isVar(param1, varMap)) {
			// First param = user declared var; Second param = string/wildcard
			unordered_set<string> result = convertSet(pkb.GetInverseProcModifies(parsedParam2));
			return getANDResult(result, getAllSet(varMap.at(param1)));
		}
		else if (!isVar(param1, varMap) && isVar(param1, varMap)) {
			// First param = const; Second param = user declared var
			unordered_set<string*>* result = pkb.GetProcModifies(parsedParam1);
			return convertSet(result);
		}
	}
	else {
		//error 
	}
}

int PQLEvaluator::parsingStmtRef(string param, unordered_map<string, ENTITY> varMap) {
	if (param.compare("_") || varMap.find(param) != varMap.end()) {
		return 0;
	}
	else {
		return stoi(param);
	}
}

string* PQLEvaluator::parsingEntRef(string* param, unordered_map<string, ENTITY> varMap) {
	if ((*param).compare("_") || varMap.find(*param) != varMap.end()) {
		return NULL;
	}
	else {
		return param;
	}
}

unordered_set<string> PQLEvaluator::convertSet(unordered_set<int>* resultSet) {
	unordered_set<string> finalResult;
	for (int k : *resultSet) {
		finalResult.insert(to_string(k));
	}

	return finalResult;
}

unordered_set<string> PQLEvaluator::convertSet(unordered_set<string*>* resultSet) {
	unordered_set<string> finalResult;
	for (string* k : *resultSet) {
		finalResult.insert(*k);
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

unordered_set<string> PQLEvaluator::getAllSet(ENTITY entity) {
	PKB pkb;
	if (entity.type == ENTITY_TYPE::statement) {
		STATEMENT_TYPE stmtType = ((STATEMENT*)(&entity))->statement_type;
		STMT_IDX_SET* result = pkb.GetAllStatements(stmtType);
		return convertSet(result);
	}
	else if (entity.type == ENTITY_TYPE::variable) {
		return convertSet(pkb.GetAllVariables());
	}
	else if (entity.type == ENTITY_TYPE::proc) {
		return convertSet(pkb.GetAllProcedures());
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

bool PQLEvaluator::isVar(string var, unordered_map<string, ENTITY> varMap) {
	if (varMap.find(var) == varMap.end()) {
		return false;
	}
	else {
		return true;
	}
}