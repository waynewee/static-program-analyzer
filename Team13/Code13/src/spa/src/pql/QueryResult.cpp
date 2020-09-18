#include "QueryResult.h"

unordered_set<string> QueryResult::getResult() {
	return this->result;
}

void QueryResult::setResult(unordered_set<string> result) {
	this->result = *(new unordered_set<string>(result));
}

bool QueryResult::isEmpty() {
	return (this->result).empty();
}