#include "QueryResult.h"

STRING_SET QueryResult::GetResult() {
	return this->result;
}

VOID QueryResult::SetResult(STRING_SET result) {
	this->result = *(new STRING_SET(result));
}

BOOLEAN QueryResult::IsEmpty() {
	return (this->result).empty();
}
