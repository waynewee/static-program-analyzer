#include "QueryInfo.h"
#include "QueryResult.h"

class PQLEvaluator
{
public:
	QueryResult evaluate(QueryInfo queryInfo);

	bool evaluateBoolSet(tuple<RELREF, string, string> func);
	int parsingStmtRef(string param);
	string* parsingEntRef(string* param);

	unordered_set<string> evaluateGetSet(tuple<RELREF, string, string> func, unordered_map<string, ENTITY> varMap, ENTITY outputVar);
	int parsingStmtRef(string param, unordered_map<string, ENTITY> varMap);
	string* parsingEntRef(string* param, unordered_map<string, ENTITY> varMap);
	unordered_set<string> convertSet(unordered_set<int>* resultSet);
	unordered_set<string> convertSet(unordered_set<string*>* resultSet);

	unordered_set<string> getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal);
	unordered_set<string> getAllSet(ENTITY entity);
	bool isContainingVar(unordered_set<string> relatedVar, unordered_set<string> keys);
	bool isVar(string var, unordered_map<string, ENTITY> varMap);
};