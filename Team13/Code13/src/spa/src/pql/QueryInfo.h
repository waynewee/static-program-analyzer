#pragma once

#include <unordered_map>

#include "CustomTypes.h"

using namespace std;

class QueryInfo
{
private:
	unordered_map<string, vector<vector<string>>> relRef_map;
	unordered_map<string, string> var_map;
	string output_var;
	bool isValid = true;
public:

	/* getters */
	unordered_map<string, vector<vector<string>>> getRelRefMap();
	unordered_map<string, string> getVarMap();
	string getOutputVar();
	bool isQueryInfoValid();

	/* setters */
	void setRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map);
	void setVarMap(unordered_map<string, string> var_map);
	void setOutputVar(string output_var);
	void setValidToFalse();
};
