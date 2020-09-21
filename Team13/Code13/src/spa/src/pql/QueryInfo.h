#pragma once

#include "PQLCustomTypes.h"

class QueryInfo
{
private:
	STRING_STRINGLISTLIST_MAP_PTR rel_ref_map;
	STRING_STRING_MAP_PTR var_map_;
	STRING_PTR output_var;
	BOOLEAN is_valid = true;
public:
	/* getters */
	STRING_STRINGLISTLIST_MAP_PTR GetRelRefMap();
	STRING_STRING_MAP_PTR GetVarMap();
	STRING_PTR GetOutputVar();
	BOOLEAN IsQueryInfoValid();

	/* setters */
	void SetRelRefMap(STRING_STRINGLISTLIST_MAP_PTR rel_ref_map);
	void SetVarMap(STRING_STRING_MAP_PTR var_map);
	void SetOutputVar(STRING_PTR output_var);
	void SetValidToFalse();

	/* printing */
	void PrintOutputVar();
	void PrintVarMap();
	void PrintRelRefMap();
};
