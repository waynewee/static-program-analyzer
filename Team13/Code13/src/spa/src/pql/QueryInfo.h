#pragma once

#include <unordered_map>

#include "PQLCustomTypes.h"

using namespace std;

class QueryInfo
{
private:
	STRING_STRINGLISTLIST_MAP st_map_;
	STRING_STRINGLISTLIST_MAP pattern_map_;
	STRINGPAIR_SET with_map_;
	STRING_STRING_MAP entity_map_;
	STRING_LIST output_list_;
	BOOLEAN is_valid_ = true;
	BOOLEAN is_invalid_due_to_semantics_ = false;
public:
	/* getters */
	STRING_STRINGLISTLIST_MAP GetStMap();
	STRING_STRINGLISTLIST_MAP GetPatternMap();
	STRINGPAIR_SET GetWithMap();
	STRING_STRING_MAP GetEntityMap();
	STRING_LIST GetOutputList();
	BOOLEAN IsQueryInfoValid();

	/* setters */
	VOID SetStMap(STRING_STRINGLISTLIST_MAP st_map);
	VOID SetPatternMap(STRING_STRINGLISTLIST_MAP pattern_map);
	VOID SetWithMap(STRINGPAIR_SET with_map);
	VOID SetEntityMap(STRING_STRING_MAP entity_map);
	VOID SetOutputList(STRING_LIST output_list);
	VOID SetValidToFalse();
	VOID SetInvalidDueToSemanticsTrue();

	/* printing */
	VOID PrintOutputList();
	VOID PrintEntityMap();
	VOID PrintClausesMap();
	VOID PrintStMap();
	VOID PrintPatternMap();
	VOID PrintWithMap();
};
