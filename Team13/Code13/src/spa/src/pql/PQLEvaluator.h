#include "QueryInfo.h"
#include "QueryResult.h"

class PQLEvaluator
{
public:
	QueryResult evaluate(QueryInfo queryInfo);

	bool evaluateNoUserDeclaredSet(string fCall, string param1, string param2);
	unordered_set<string> evaluateOneDeclaredSet(string fCall, string param);
	unordered_set<string> evaluateInverseOneDeclaredSet(string fCall, string param);
	unordered_set<vector<string>*> evaluateTwoDeclaredSet(string outputVarType);
	unordered_map<string, unordered_set<string>> consolidateResults(string currCheck, unordered_set<string> relatedVar, unordered_map<string, 
		unordered_set<string>> consolidatedResults, unordered_map<string, unordered_set<string>> oneUserResultSet, unordered_map<vector<string>*, 
		unordered_set<vector<string>*>> twoUserResultSet);

	unordered_set<string> getNewResult(unordered_set<vector<string>*> value, int positionToCheck);
	unordered_set<string> getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal);
	unordered_set<vector<string>*> getANDResult(unordered_set<vector<string>*> existingVal, unordered_set<vector<string>*> newVal);
	unordered_set<string> getANDResult(unordered_set<string> r1, unordered_set<vector<string>*> r2, int positionToCheck);
	unordered_set<string> getAllSet(string outputVarType);
	STATEMENT_TYPE getStmtType(string outputVarType);

	bool removeIrrelevant(unordered_set<string>* value, unordered_set<string> tmp);
	bool removeIrrelevant(unordered_set<vector<string>*>* value, unordered_set<string> tmp, int positionToCheck);

	unordered_set<string> convertSet(unordered_set<int> resultSet);
	unordered_set<string> convertSet(unordered_set<string*>* resultSet);
	unordered_set<string> convertSet(unordered_set<double>* resultSet);
	unordered_set<vector<string>*> convertSet(STMT_STMT_PAIR_SET resultSet);
	unordered_set<vector<string>*> convertSet(STMT_VAR_PAIR_SET resultSet);
	unordered_set<vector<string>*> convertSet(PROC_VAR_PAIR_SET resultSet);

	bool isWildCard(string var);
	bool isVar(string var, unordered_map<string, string> varMap);
	vector<string>* isKey(vector<string> key, unordered_map<vector<string>*, unordered_set<vector<string>*>> twoUserResultSet);

	int parsingStmtRef(string param);
	string* parsingEntRef(string param);
};