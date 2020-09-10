#include <string>
#include <unordered_set>

using namespace std;

class QueryResult
{
public:
	unordered_set<string> result;

	unordered_set<string> getResult();
	void setResult(unordered_set<string> result);
	bool isEmpty();
};
