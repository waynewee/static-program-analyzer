#include "QueryInfo.h"
#include "QueryResult.h"

class PQLEvaluator
{
public:
	QueryResult evaluate(QueryInfo queryInfo);

	bool isWildCard(string var);
	bool isVar(string var, unordered_map<string, string> varMap);
	string* isKey(vector<string> key, unordered_map<vector<string>*, unordered_set<string>> twoUserResultSet);

	bool evaluateNoUserDeclaredSet(vector<string> func);
	unordered_set<string> evaluateOneDeclaredSet(string fCall, string param);
	unordered_set<string> evaluateInverseOneDeclaredSet(string fCall, string param);
	unordered_set<string> evaluateTwoDeclaredSet(string outputVarType);

	int parsingStmtRef(string param);
	string* parsingEntRef(string param);
	
	unordered_set<string> convertSet(unordered_set<int> resultSet);
	unordered_set<string> convertSet(unordered_set<string*>* resultSet);
	unordered_set<string> convertSet(unordered_set<double>* resultSet);
	unordered_set<string> convertSet(STMT_STMT_PAIR_SET resultSet);
	unordered_set<string> convertSet(STMT_VAR_PAIR_SET resultSet);
	unordered_set<string> convertSet(PROC_VAR_PAIR_SET resultSet);
	
	unordered_set<string> getANDResult(unordered_set<string> existingVal, unordered_set<string> newVal);

	unordered_set<string> getAllSet(string outputVarType);


	// vector<unordered_set<string>*> evaluateGetSet(vector<string> func, unordered_map<string, string> varMap, string outputVarType);

	bool isContainingVar(unordered_set<string> relatedVar, unordered_set<string> keys);
	bool isStmtType(string type);
	// unordered_set<string> evaluateSynonymWildCardType(string fCall);
	// unordered_set<string> evaluateWildCardSynonymType(string fCall);
	// unordered_set<string> evaluateIntSynonymType(string fCall, string value);

	//int parsingStmtRef(string param, unordered_map<string, string> varMap);
	//string* parsingEntRef(string param, unordered_map<string, string> varMap);
};