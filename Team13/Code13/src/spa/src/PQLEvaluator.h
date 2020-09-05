#include <string>
using namespace std;

#include "QueryInfo.h"
#include "QueryResult.h"

class PQLEvaluator
{
public:
	QueryResult evaluate(QueryInfo queryInfo);

	vector<string> getParsingParams(vector<string> params);
	unordered_set<string> getParamToKey(vector<string> params);
	unordered_set<string> getFuncApi(string relRef, vector<string> params);
	unordered_set<string> getAllApi(string entity_type);
	unordered_set<string> getANDResults(unordered_set<string> existingVal, unordered_set<string> newVal);
	bool isContainingVar(unordered_set<string> var, unordered_set<string> keys);
	bool isConst(string s);
};