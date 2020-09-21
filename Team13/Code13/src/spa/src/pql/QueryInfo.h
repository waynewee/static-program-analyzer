#pragma once

#include <unordered_map>

#include "PQLCustomTypes.h"

using namespace std;

class QueryInfo
{
private:
	STRING_STRINGLISTLIST_MAP rel_ref_map_;
	STRING_STRING_MAP var_map_;
	STRING output_var_;
	BOOLEAN is_valid_ = true;
public:
	/* getters */
	STRING_STRINGLISTLIST_MAP GetRelRefMap();
	STRING_STRING_MAP GetVarMap();
	STRING GetOutputVar();
	BOOLEAN IsQueryInfoValid();

	/* setters */
	VOID SetRelRefMap(STRING_STRINGLISTLIST_MAP rel_ref_map);
	VOID SetVarMap(STRING_STRING_MAP var_map);
	VOID SetOutputVar(STRING output_var);
	VOID SetValidToFalse();

	/* printing */
	VOID PrintOutputVar();
	VOID PrintVarMap();
	VOID PrintRelRefMap();
};
