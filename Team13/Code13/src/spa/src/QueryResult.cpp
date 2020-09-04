#include <string>
using namespace std;
#include "QueryResult.h"

string QueryResult::getResult() {
	return this->result;
}

void QueryResult::setResult(string result) {
	this->result = result;
}