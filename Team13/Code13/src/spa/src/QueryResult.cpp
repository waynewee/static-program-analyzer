#include <string>
using namespace std;

#include "QueryResult.h"

unordered_set<string> QueryResult::getResult() {
	return this->result;
}

void QueryResult::setResult(unordered_set<string> result) {
	this->result = result;
}

bool QueryResult::isEmpty() {
	return result.empty();
}