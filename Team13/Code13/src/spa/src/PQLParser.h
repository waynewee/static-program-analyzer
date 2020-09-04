#include <string>
using namespace std;
#pragma once
class PQLParser
{
public:
	void parse(string s);

	string trimLeadingWhitespaces(const string& s);

	string trimTrailingWhitespaces(const string& s);
};

