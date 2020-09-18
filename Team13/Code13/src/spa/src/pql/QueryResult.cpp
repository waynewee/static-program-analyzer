#include "QueryResult.h"

STRING_SET_PTR QueryResult::getResult() {
	return this->result;
}

void QueryResult::setResult(STRING_SET_PTR result) {
	this->result = result;
}

BOOLEAN QueryResult::isEmpty() {
	return this->result->empty();
}