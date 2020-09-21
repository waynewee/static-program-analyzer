#include "PQLProjector.h"

#include <sstream>

STRING PQLProjector::Project(QueryResult query_result) {
	STRING_SET result = query_result.GetResult();

	stringstream inter_result;
	copy(result.begin(), result.end(), ostream_iterator<STRING>(inter_result, " "));
	
	STRING final_result = inter_result.str();
	if (!final_result.empty()) {
		final_result.resize(final_result.length() - 1); // trim trailing space
	}
	
	return final_result;
}