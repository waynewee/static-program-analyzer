#pragma once

#include <unordered_map>

#include "CustomTypes.h"

using namespace std;

class QueryInfo
{
private:
	unordered_map<RELREF, vector<vector<VAR_NAME>>> relRef_map;
	unordered_map<VAR_NAME,ENTITY> var_map;
	VAR_NAME output_var;
public:
	/* getters */
	unordered_map<RELREF, vector<vector<VAR_NAME>>> getRelRefMap();
	unordered_map<string, ENTITY> getVarMap();
	VAR_NAME getOutputVar();

	/* setters */
	void setRelRefMap(unordered_map<RELREF, vector<vector<VAR_NAME>>> relRef_map);
	void setVarMap(unordered_map<string, ENTITY> var_map);
	void setOutputVar(VAR_NAME output_var);
};