#include <CustomTypes.h>
#include <PQLCustomTypes.h>
#include <QueryInfo.h>
#include <QueryResult.h>
// #include <PQLOptimization.h>

class PQLEvaluator
{
	// typedef priority_queue< STRING_LIST, STRINGLIST_LIST, PQLOptimization> FUNCTION_QUEUE;
	// typedef unordered_map<STRING_SET*, FUNCTION_QUEUE> STRINGSET_FUNCTIONQUEUE_MAP;
public:
	QueryResult Evaluate(QueryInfo query_info);

	QueryResult SetResult(BOOLEAN is_boolean_output, STRING bool_result, STRINGLIST_SET result);
		
	BOOLEAN ParseClauses(QueryInfo query_info, STRINGPAIR_SET* constraints);
	BOOLEAN ParseClauses(QueryInfo query_info, STRINGSET_STRINGLISTSET_MAP* synonyms_map);

	STRING_SET* GetRelatedSynonyms(STRING synonym, STRINGSET_STRINGLISTSET_MAP synonyms_map);
	STRING_SET* GetRelatedSynonyms(STRING_SET synonyms, STRINGSET_STRINGLISTSET_MAP* synonyms_map);
	STRING_LIST* GetRelatedSynonyms(STRING synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map);
	STRING_LIST* GetRelatedSynonyms(STRING_LIST synonym, STRINGLIST_STRINGLISTSET_MAP synonyms_map);

	BOOLEAN EvaluateNoSynonymSet(STRING f_call, STRING param1, STRING param2);
	STRINGLIST_SET EvaluatePatternCall(STRING f_call, STRING param1, STRING param2, STRING type);
	STRINGLIST_SET EvaluatePatternCall(STRING f_call, STRING param, STRING type);
	STRINGLIST_SET EvaluateOneSynonymSet(STRING f_call, STRING param);
	STRINGLIST_SET EvaluateInverseOneSynonymSet(STRING f_call, STRING param);
	STRINGLIST_SET EvaluateTwoSynonymSet(STRING output_var_type);
	STRINGLIST_SET EvaluateAllCall(STRING output_var_type);

	STRINGLIST_SET ConvertSet(INTEGER_SET result_set);
	STRINGLIST_SET ConvertSet(DOUBLE_SET result_set);
	STRINGLIST_SET ConvertSet(STRING_SET result_set);
	STRINGLIST_SET ConvertSet(STMT_STMT_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(STMT_VAR_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(PROC_VAR_PAIR_LIST result_set);
	STRINGLIST_SET ConvertSet(PROC_PROC_PAIR_LIST result_set);

	// VOID CheckConstraints(STRINGPAIR_SET constraints, STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST key, STRING_SET* value);
	BOOLEAN CheckConstraints(STRINGPAIR_SET constraints, STRING_STRING_MAP entity_map, STRING_LIST key, STRINGLIST_SET* value);
	
	BOOLEAN RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, INTEGER pos_to_check);

	STRING_SET GetIntersectResult(STRING_SET existing_val, STRING_SET new_val);
	STRINGLIST_SET GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, INTEGER pos_to_check);
	
	STRING_SET GetNewResult(STRINGLIST_SET value, INTEGER pos_to_check);
	STRINGLIST_SET GetNewResult(STRINGLIST_SET value, INTEGER_SET pos_to_check);
	
	INTEGERLIST_LIST GetCommonSynonymsIndex(STRING_LIST large_keys, STRING_LIST small_keys);
	
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET large_values, STRINGLIST_SET small_values, INTEGERLIST_LIST indexes);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET results, STRINGLIST_SET to_combine);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET results, STRINGLIST_SET values, INTEGER pos_to_add, INTEGERPAIR_SET to_check);
	
	VOID AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP* results_map);

	BOOLEAN IsVar(STRING var);
	BOOLEAN IsString(STRING var);
	BOOLEAN IsInteger(STRING var);
	BOOLEAN IsUnderscore(STRING var);
	BOOLEAN IsBooleanOutput(STRING_LIST output_list);
	BOOLEAN IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list);

	INTEGER ParsingStmtRef(STRING param);
	STRING ParsingEntRef(STRING param);
};
