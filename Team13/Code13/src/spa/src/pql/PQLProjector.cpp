#include "PQLProjector.h"

#include <sstream>

string PQLProjector::project(QueryResult queryResult) {
	unordered_set<string> result = queryResult.getResult();

	stringstream interResult;
	copy(result.begin(), result.end(), ostream_iterator<string>(interResult, " "));
	
	string finalResult = interResult.str();
	if (!finalResult.empty()) {
		finalResult.resize(finalResult.length() - 1); // trim trailing space
	}
	
	return finalResult;
}