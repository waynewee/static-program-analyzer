#include "QueryInfo.h"new_val
#include "QueryResult.h"

class PQLEvaluator
{
public:
	QueryResult Evaluate(QueryInfo query_info);

	BOOLEAN EvaluateNoUserDeclaredSet(STRING f_call, STRING param1, STRING param2);
	STRING_SET EvaluateOneDeclaredSet(STRING f_call, STRING param);
	STRING_SET EvaluateInverseOneDeclaredSet(STRING f_call, STRING param);
	STRINGLIST_SET EvaluateTwoDeclaredSet(STRING output_var_type);
	STRING_STRINGSET_MAP ConsolidateResults(STRING curr_check, STRING_SET related_var, 
		STRING_STRINGSET_MAP consolidated_results, STRING_STRINGSET_MAP one_user_result_set, 
		STRINGLIST_STRINGLISTSET_MAP two_user_result_set);

	STRING_SET GetNewResult(STRINGLIST_SET value, INTEGER pos_to_check);
	STRING_SET GetIntersectResult(STRING_SET existing_val, STRING_SET new_val);
	STRINGLIST_SET GetIntersectResult(STRINGLIST_SET existing_val, STRINGLIST_SET new_val);
	STRING_SET GetIntersectResult(STRING_SET r1, STRINGLIST_SET r2, INTEGER pos_to_check);
	STRING_SET GetAllSet(STRING output_var_type);
	STATEMENT_TYPE GetStmtType(STRING output_var_type);

	BOOLEAN RemoveIrrelevant(STRING_SET value, STRING_SET tmp);
	BOOLEAN RemoveIrrelevant(STRINGLIST_SET value, STRING_SET tmp, INTEGER pos_to_check);

	STRING_SET ConvertSet(INTEGER_SET result_set);
	STRING_SET ConvertSet(unordered_set<string*>* result_set); // DO I STILL NEED IT??
	STRING_SET ConvertSet(DOUBLE_SET result_set);
	STRINGLIST_SET ConvertSet(STMT_STMT_PAIR_SET result_set);
	STRINGLIST_SET ConvertSet(STMT_VAR_PAIR_SET result_set);
	STRINGLIST_SET ConvertSet(PROC_VAR_PAIR_SET result_set);

	BOOLEAN IsWildCard(STRING var);
	BOOLEAN IsVar(STRING var, STRING_STRING_MAP var_map);
	STRING_LIST* IsKey(STRING_LIST key, STRINGLIST_STRINGLISTSET_MAP two_user_result_set);

	INTEGER ParsingStmtRef(STRING param);
	STRING ParsingEntRef(STRING param);
};