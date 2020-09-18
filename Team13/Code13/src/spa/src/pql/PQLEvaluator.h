#include "QueryInfo.h"
#include "QueryResult.h"
#include "PQLCustomTypes.h"
#include "CustomTypes.h"

class PQLEvaluator
{
public:
	QueryResult* Evaluate(QueryInfo* query_info);

	BOOLEAN EvaluateNoUserDeclaredSet(STRING_PTR f_call, STRING_PTR param1, STRING_PTR param2);
	STRING_SET_PTR EvaluateOneDeclaredSet(STRING_PTR f_call, STRING_PTR param);
	STRING_SET_PTR EvaluateInverseOneDeclaredSet(STRING_PTR f_call, STRING_PTR param);
	STRINGLIST_SET_PTR EvaluateTwoDeclaredSet(STRING_PTR f_call);
	STRING_STRINGSET_MAP_PTR ConsolidateResults(STRING_PTR curr_check, STRING_SET_PTR related_var, 
		STRING_STRINGSET_MAP_PTR consolidated_results,	STRING_STRINGSET_MAP_PTR one_user_result_set, 
		STRINGLIST_STRINGLISTSET_MAP_PTR two_user_result_set);

	STRING_SET_PTR GetAllSet(STRING_PTR output_var_type);
	STATEMENT_TYPE GetStmtType(STRING_PTR output_var_type);

	STRING_SET_PTR GetIntersectResult(STRING_SET_PTR r1, STRINGLIST_SET_PTR r2, INTEGER pos_to_check);
	STRING_SET_PTR GetIntersectResult(STRING_SET_PTR existing_val, STRING_SET_PTR new_val);
	STRINGLIST_SET_PTR GetIntersectResult(STRINGLIST_SET_PTR existing_val, STRINGLIST_SET_PTR new_val);
	STRING_SET_PTR GetNewResult(STRINGLIST_SET_PTR value, INTEGER pos_to_check);

	BOOLEAN RemoveIrrelevant(STRING_SET_PTR value, STRING_SET_PTR tmp);
	BOOLEAN RemoveIrrelevant(STRINGLIST_SET_PTR value, STRING_SET_PTR tmp, INTEGER pos_to_check);

	STRING_SET_PTR ConvertSet(INTEGER_SET_PTR result_set);
	STRING_SET_PTR ConvertSet(STRING_SET_PTR result_set);
	STRING_SET_PTR ConvertSet(DOUBLE_SET_PTR result_set);
	STRINGLIST_SET_PTR ConvertSet(STMT_STMT_PAIR_SET* result_set);
	STRINGLIST_SET_PTR ConvertSet(STMT_VAR_PAIR_SET* result_set);
	STRINGLIST_SET_PTR ConvertSet(PROC_VAR_PAIR_SET* result_set);

	BOOLEAN IsWildCard(STRING_PTR var);
	BOOLEAN IsVar(STRING_PTR var, STRING_STRING_MAP_PTR var_map);
	STRING_LIST_PTR IsKey(STRING_LIST_PTR key, STRINGLIST_STRINGLISTSET_MAP_PTR two_user_result_set);

	INTEGER ParsingStmtRef(STRING_PTR param);
	STRING_PTR ParsingEntRef(STRING_PTR param);
};