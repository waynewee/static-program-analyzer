#include "PQLProjector.h"

#include <sstream>

STRING_SET PQLProjector::Project(QueryResult query_result) {
	STRING_SET result = query_result.GetResult();

	/*
	stringstream inter_result;
	copy(result.begin(), result.end(), ostream_iterator<STRING>(inter_result, ", "));

	STRING final_result = inter_result.str();

	if (!final_result.empty()) {
		final_result = final_result.substr(0, final_result.length() - 2); // trim trailing space
	}
	*/

	return result;
}
