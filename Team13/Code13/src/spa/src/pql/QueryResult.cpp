#include "QueryResult.h"

STRING_SET_PTR QueryResult::GetResult() {
	return this->result;
}

void QueryResult::SetResult(STRING_SET_PTR result) {
	this->result = result;
}

BOOLEAN QueryResult::isEmpty() {
	return this->result->empty();
}