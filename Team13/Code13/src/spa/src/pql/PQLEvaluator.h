#include "QueryInfo.h"
#include "QueryResult.h"
#include "pql/PQLCustomTypes.h"
#include "CustomTypes.h"

class PQLEvaluator
{
public:
	QueryResult* evaluate(QueryInfo* queryInfo);

	BOOLEAN evaluateNoUserDeclaredSet(STRING_PTR fCall, STRING_PTR param1, STRING_PTR param2);
	STRING_SET_PTR evaluateOneDeclaredSet(STRING_PTR fCall, STRING_PTR param);
	STRING_SET_PTR evaluateInverseOneDeclaredSet(STRING_PTR fCall, STRING_PTR param);
	STRINGLIST_SET_PTR evaluateTwoDeclaredSet(STRING_PTR outputVarType);
	STRING_STRINGSET_MAP_PTR consolidateResults(STRING_PTR currCheck, STRING_SET_PTR relatedVar, 
		STRING_STRINGSET_MAP_PTR consolidatedResults,	STRING_STRINGSET_MAP_PTR oneUserResultSet, 
		STRINGLIST_STRINGLISTSET_MAP_PTR twoUserResultSet);

	STRING_SET_PTR getNewResult(STRINGLIST_SET_PTR value, INTEGER positionToCheck);
	STRING_SET_PTR getANDResult(STRING_SET_PTR existingVal, STRING_SET_PTR newVal);
	STRINGLIST_SET_PTR getANDResult(STRINGLIST_SET_PTR existingVal, STRINGLIST_SET_PTR newVal);
	STRING_SET_PTR getANDResult(STRING_SET_PTR r1, STRINGLIST_SET_PTR r2, INTEGER positionToCheck);
	STRING_SET_PTR getAllSet(STRING_PTR outputVarType);
	STATEMENT_TYPE getStmtType(STRING_PTR outputVarType);

	BOOLEAN removeIrrelevant(STRING_SET_PTR value, STRING_SET_PTR tmp);
	BOOLEAN removeIrrelevant(STRINGLIST_SET_PTR value, STRING_SET_PTR tmp, INTEGER positionToCheck);

	STRING_SET_PTR convertSet(INTEGER_SET_PTR resultSet);
	STRING_SET_PTR convertSet(STRING_SET_PTR resultSet);
	STRING_SET_PTR convertSet(DOUBLE_SET_PTR resultSet);
	STRINGLIST_SET_PTR convertSet(STMT_STMT_PAIR_SET resultSet);
	STRINGLIST_SET_PTR convertSet(STMT_VAR_PAIR_SET resultSet);
	STRINGLIST_SET_PTR convertSet(PROC_VAR_PAIR_SET resultSet);

	BOOLEAN isWildCard(STRING_PTR var);
	BOOLEAN isVar(STRING_PTR var, STRING_STRING_MAP_PTR varMap);
	STRING_LIST_PTR isKey(STRING_LIST_PTR key, STRINGLIST_STRINGLIST_MAP_PTR twoUserResultSet);

	INTEGER parsingStmtRef(STRING_PTR param);
	STRING_PTR parsingEntRef(STRING_PTR param);
};