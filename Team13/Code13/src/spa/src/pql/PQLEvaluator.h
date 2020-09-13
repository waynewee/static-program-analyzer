#include "QueryInfo.h"
#include "QueryResult.h"

class PQLEvaluator
{
public:
	QueryResult evaluate(QueryInfo queryInfo);

	bool evaluateBoolSet(tuple<string, string, string> func);
	int parsingStmtRef(string param);
	string parsingEntRef(string param);

	unordered_set<string> evaluateGetSet(tuple<string, string, string> func, unordered_map<string, string> varMap, string outputVar);
	int parsingStmtRef(string param, unordered_map<string, string> varMap);
	string parsingEntRef(string param, unordered_map<string, string> varMap);
	unordered_set<string> convertSet(unordered_set<int> resultSet);
	unordered_set<string> convertSet(unordered_set<string*>* resultSet);
	unordered_set<string> convertSet(unordered_set<double>* resultSet);

	unordered_set<string> getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal);
	unordered_set<string> getAllSet(string outputVarType);
	bool isContainingVar(unordered_set<string> relatedVar, unordered_set<string> keys);
	bool isVar(string var, unordered_map<string, string> varMap);
	bool isStmtType(string type);
};