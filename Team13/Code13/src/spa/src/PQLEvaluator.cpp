#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#include "PQLEvaluator.h"

QueryResult PQLEvaluator::evaluate(QueryInfo queryInfo) {
	unordered_map<unordered_set<string>, unordered_set<string>> interResults;
	QueryResult finalResult;

	unordered_map<string, string> varMap = queryInfo.getVarMap();
	unordered_map<string, vector<vector<string>>> relRefMap = queryInfo.getRelRefMap();
	string outputVar = queryInfo.getOutputVar();

	// Adding only output variable
	string entityType = varMap.at(outputVar);
	unordered_set<string> key = { outputVar };
	unordered_set<string> value = getAllApi(entityType);
	interResults.insert(key, value);

	// Function calling
	for (auto f = relRefMap.cbegin(); f != relRefMap.cend(); f++) {
		string fCall = (*f).first;

		vector<vector<string>> allParams = (*f).second;

		for (vector<string> p : allParams) {
			vector<string> params = getParsingParams(p);

			unordered_set<string> value = getFuncApi(fCall, p);
			if (value.empty()) {
				finalResult.setResult({});
				return finalResult;
			}

			unordered_set<string> key;
			if (fCall.compare("Pattern") || fCall.compare("pattern")) {
				key = { p[2] };
			} else {
				key = getParamToKey(p);
			}
			 
			if (!key.empty() && interResults.find(key) == interResults.end()) {
				interResults.insert(key, value);
			}
			else if (!key.empty()) {
				interResults[key] = getANDResults(value, interResults.at(key));
			}
		}
	}

	// Consolidate results
	unordered_set<string> varToInclude = {outputVar};
	for (auto k = interResults.cbegin(); k != interResults.cend(); k++) {
		if (isContainingVar(varToInclude, (*k).first)) {

			varToInclude.insert((*k).first.begin(), (*k).first.end());

			if (finalResult.isEmpty()) {
				finalResult.setResult((*k).second);
			}
			else {
				finalResult.setResult(getANDResults((*k).second, finalResult.getResult()));
			}
		}
	}

	return finalResult;
}

bool PQLEvaluator::isContainingVar(unordered_set<string> var, unordered_set<string> keys) {
	bool found = false;

	for (auto i = keys.begin(); i != keys.end(); i++) {
		if (var.find(*i) != var.end()) {
			found = true;
			return found;
		}
	}

	return found;
}

unordered_set<string> PQLEvaluator::getANDResults(unordered_set<string> existingVal, unordered_set<string> newVal) {
	unordered_set<string> result = {};

	unordered_set<string> loop = existingVal;
	unordered_set<string> notLoop = newVal;
	if (existingVal.size() > newVal.size()) {
		loop = newVal;
		notLoop = existingVal;
	}

	for (auto i = loop.begin(); i != loop.end(); i++) {
		if (notLoop.find(*i) != notLoop.end()) 
			result.insert(*i);
	}

	return result;
}

vector<string> PQLEvaluator::getParsingParams(vector<string> params) {
	vector<string> parsingParams = { NULL, NULL };

	if (isConst(params[0])) {
		parsingParams[0] = params[0];
	} 

	if (isConst(params[1])) {
		parsingParams[1] = params[1];
	}

	return parsingParams;
}

unordered_set<string> PQLEvaluator::getParamToKey(vector<string> params) {
	unordered_set<string> key = {};

	if (!isConst(params[0]) && !params[0].compare("null")) {
		key.insert(params[0]);
	}

	if (!isConst(params[1]) && !params[1].compare("null")) {
		key.insert(params[1]);
	}

	return key;
}

unordered_set<string> PQLEvaluator::getFuncApi(string relRef, vector<string> params) {
	if (relRef.compare("Follows")) {
		/*return PKB_API.getFollows(params[0], params[1]);*/
	}
	else if (relRef.compare("FollowsT")) {
		/*return PKB_API.getFollowsT(params[0], params[1]);*/
	}
	else if (relRef.compare("Parent")) {
		/*return PKB_API.getParent(params[0], params[1]);*/
	}
	else if (relRef.compare("ParentT")) {
		/*return PKB_API.getParentT(params[0], params[1]);*/
	}
	else if (relRef.compare("UsesS")) {
		/*return PKB_API.getUsesS(params[0], params[1]);*/
	}
	else if (relRef.compare("UsesP")) {
		/*return PKB_API.getUsesP(params[0], params[1]);*/
	}
	else if (relRef.compare("ModifiesS")) {
		/*return PKB_API.getModifiesS(params[0], params[1]);*/
	}
	else if (relRef.compare("ModifiesP")) {
		/*return PKB_API.getModifiesP(params[0], params[1]);*/
	}
	else if (relRef.compare("Pattern") || relRef.compare("pattern")) {
		/*return PKB_API.getPattern(params[0], params[1]);*/
	}
	else {
		/* error */
	}
}

unordered_set<string> PQLEvaluator::getAllApi(string entity_type) {
	list<string> returnStmts = {"stmt", "read", "print", "call", "while", "if", "assign"};
	
	if (find(returnStmts.begin(), returnStmts.end(), entity_type) != returnStmts.end()) {
		/*return PKB_API.getAllStmt(null);*/
	}
	else if (entity_type.compare("variable")) {
		/*return PKB_API.getAllVar(null);*/
	}
	else if (entity_type.compare("procedure")) {
		/*return PKB_API.getAllProc(null);*/
	}
	else if (isConst(entity_type)) {
		return { entity_type };
	}
	else {
		/* error */
	}
}

bool PQLEvaluator::isConst(string s) {
	return find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}