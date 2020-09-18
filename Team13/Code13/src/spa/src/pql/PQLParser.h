#include <string>
using namespace std;
#include "QueryInfo.h"

#pragma once
class PQLParser
{
public:
	QueryInfo parse(string s);

	void buildQueryInfo(QueryInfo queryInfo, string s);

	string trimLeadingWhitespaces(const string& s);

	string trimTrailingWhitespaces(const string& s);

};

