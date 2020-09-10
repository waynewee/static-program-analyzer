#include <string>
using namespace std;

class PQLParser
{
public:
	void parse(string s);

	string trimLeadingWhitespaces(const string& s);

	string trimTrailingWhitespaces(const string& s);
};

