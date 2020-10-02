#pragma once

#include <unordered_map>

#include "PQLCustomTypes.h"

using namespace std;

class QueryInfo
{
private:
	STRING_STRINGLISTLIST_MAP st_map_;
	STRING_STRINGLISTLIST_MAP p_pattern_map_;
	STRING_STRINGLISTLIST_MAP f_pattern_map_;
	STRING_STRINGLIST_MAP with_map_;
	STRING_STRING_MAP entity_map_;
	STRING_LIST output_list_;
	BOOLEAN is_valid_ = true;
public:
	/* getters */
	STRING_STRINGLISTLIST_MAP GetStMap();
	STRING_STRINGLISTLIST_MAP GetPPatternMap();
	STRING_STRINGLISTLIST_MAP GetFPatternMap();
	STRING_STRINGLIST_MAP GetWithMap();
	STRING_STRING_MAP GetEntityMap();
	STRING_LIST GetOutputList();
	BOOLEAN IsQueryInfoValid();

	/* setters */
	VOID SetStMap(STRING_STRINGLISTLIST_MAP st_map);
	VOID SetPPatternMap(STRING_STRINGLISTLIST_MAP p_pattern_map);
	VOID SetFPatternMap(STRING_STRINGLISTLIST_MAP f_pattern_map);
	VOID SetWithMap(STRING_STRINGLIST_MAP with_map);
	VOID SetEntityMap(STRING_STRING_MAP entity_map);
	VOID SetOutputList(STRING_LIST output_list);
	VOID SetValidToFalse();

	/* printing */
	VOID PrintOutputList();
	VOID PrintEntityMap();
	VOID PrintClausesMap();
	VOID PrintStMap();
	VOID PrintPPatternMap();
	VOID PrintFPatternMap();
	VOID PrintWithMap();
};
