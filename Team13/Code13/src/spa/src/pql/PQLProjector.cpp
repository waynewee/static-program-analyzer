#include "PQLProjector.h"

#include <sstream>

STRING_PTR PQLProjector::project(QueryResult* query_result) {
	
	STRING_SET_PTR result = query_result->GetResult();

	STRING_PTR final_result = new STRING();
	for (STRING_PTR s : *result) {
		final_result->append(*s);
		final_result->append(" ");
	}

	if (!final_result->empty()) {
		final_result->resize(final_result->length() - 1); // trim trailing space
	}
	
	return final_result;
}
