#pragma once

#include "PQLCustomTypes.h"

class QueryInfo
{
private:
	STRING_STRINGLISTLIST_MAP_PTR relRef_map;
	STRING_STRING_MAP_PTR var_map;
	STRING_PTR output_var;
	BOOLEAN isValid = true;
public:
	/* getters */
	STRING_STRINGLISTLIST_MAP_PTR getRelRefMap();
	STRING_STRING_MAP_PTR getVarMap();
	STRING_PTR getOutputVar();
	BOOLEAN isQueryInfoValid();

	/* setters */
	void setRelRefMap(STRING_STRINGLISTLIST_MAP_PTR relRef_map);
	void setVarMap(STRING_STRING_MAP_PTR var_map);
	void setOutputVar(STRING_PTR output_var);
	void setValidToFalse();

	/* printing */
	// void printOutputVar();
	// void printVarMap();
	// void printRelRefMap();
};
