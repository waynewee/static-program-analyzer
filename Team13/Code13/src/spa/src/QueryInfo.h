#include <unordered_set>
#include <unordered_map>
#include <string>
using namespace std;

#pragma once
class QueryInfo
{
public:
	unordered_map<string, string> var_map;
	unordered_map<string, vector<vector<string>>> relRef_map;
	vector<string> output_var;

	/* getters */
	unordered_map<string, vector<vector<string>>> getRelRefMap();
	unordered_map<string, string> getVarMap();
	vector<string> getOutputVar();

	/* setters */
	void setRelRefMap(unordered_map<string, vector<vector<string>>> relRef_map);
	void setVarMap(unordered_map<string, string> var_map);
	void setOutputVar(vector<string> output_var);
};

