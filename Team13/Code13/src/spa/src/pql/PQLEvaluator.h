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

	VOID Print(STRING_LIST to_print);
	VOID Print(STRING_SET to_print);
	VOID Print(STRINGLIST_SET to_print);
	VOID Print(STRINGLIST_STRINGLISTSET_MAP to_print);
	VOID Print(STRINGSET_STRINGLISTSET_MAP to_print);

	VOID AddResult(STRING_LIST key, STRINGLIST_SET value, STRINGLIST_STRINGLISTSET_MAP* results_map);
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

	BOOLEAN CheckConstraints(STRINGPAIR_SET* constraints, STRING_STRING_MAP entity_map, STRINGLIST_STRINGLISTSET_MAP* results_map, STRING_LIST key, STRINGLIST_SET* value);

	BOOLEAN RemoveIrrelevant(STRINGLIST_SET* value, STRINGLIST_SET tmp, INTEGER pos_to_check);
	// BOOLEAN RemoveIrrelevant(STRINGLIST_SET* value, STRING tmp, INTEGER pos_to_check);

	INTEGER GetCommonSynonymsIndex(STRING_LIST large_keys, STRING synonym);
	INTEGERPAIR_SET GetCommonSynonymsIndex(STRING_LIST large_keys, STRING_LIST small_keys);
	STRING_SET GetNewResult(STRINGLIST_SET value, INTEGER pos_to_check);
	STRINGLIST_SET GetNewResult(STRINGLIST_SET value, INTEGER_LIST pos_to_check);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET large_values, STRINGLIST_SET small_values, INTEGERLIST_LIST indexes);
	STRINGLIST_SET GetCombinedResult(STRINGLIST_SET output_result, STRINGLIST_SET result, INTEGER pos_to_compare);
	STRINGLIST_SET GetCartesianProduct(STRINGLIST_STRINGLISTSET_MAP results_map, STRING_LIST output_list);
	STRINGLIST_SET GetNoDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values);
	STRINGLIST_SET GetDependencyProduct(STRINGLIST_SET results, STRINGLIST_SET values, INTEGER pos_to_add, INTEGERPAIR_SET to_check);
	
	STRING_SET GetIntersectResult(STRING_SET val1, STRING_SET val2);
	STRING_SET GetIntersectResult(STRING_SET val1, STRINGLIST_SET val2, INTEGER pos_to_check);
	STRINGLIST_SET GetIntersectResult(STRINGLIST_SET val1, STRINGLIST_SET val2);

	STRING_SET GetAlternateResult(STRING values, STRING type);
	// STRING_SET GetAlternateResult(STRING_SET values, STRING type);
	STRING_SET GetAlternateResult(STRINGLIST_SET values, STRING type);
	STRINGLIST_SET GetAlternateOutputResult(STRING_SET values, STRING type);

	BOOLEAN IsVar(STRING var);
	BOOLEAN IsString(STRING var);
	BOOLEAN IsInteger(STRING var);
	BOOLEAN IsUnderscore(STRING var);
	BOOLEAN IsBooleanOutput(STRING_LIST output_list);
	BOOLEAN IsOutputSynonyms(STRING_LIST synonyms, STRING_LIST output_list);
	BOOLEAN IsDuplicate(STRINGLIST_SET set, STRING_LIST value);
	BOOLEAN IsDependencyStatisfied(STRING_LIST result1, STRING_LIST result2, INTEGERPAIR_SET to_check);
	BOOLEAN IsSameEntityType(STRING type, STRING check);

	INTEGER ParsingStmtRef(STRING param);
	STRING ParsingEntRef(STRING param);
	STRING ParsingSynonym(STRING synonym);
	STRING ParsingSynonymAttribute(STRING synonym);
}; 
