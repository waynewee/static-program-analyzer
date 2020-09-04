#include <unordered_set>
#include <unordered_map>
#include <string>
using namespace std;

#pragma once
class QueryInfo
{
public:
	unordered_set<string> assign_declarations;

	unordered_map<string, vector<string>> relRef_map;

	void setRelRefMap(unordered_map<string, vector<string>> relRef_map);
};

