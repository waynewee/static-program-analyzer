#include "PKB.h"
#include "PQLDriver.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "PQLOptimizedEvaluator.h"
#include "QueryInfo.h"
#include "QueryResult.h"
#include <iostream>

STRING_SET PQLDriver::Query(string query_string) {
	PQLParser parser = PQLParser();
	PQLProjector projector = PQLProjector();

	STRING_SET final_result;

	if (DEBUG) {
		cout << "Query: " << query_string << endl;
	}
	QueryInfo parsed_info = parser.Parse(query_string);
	QueryResult result = QueryResult();

	if (!parsed_info.IsQueryInfoValid()) {
		// Query invalid - get false result: bool/empty
		result.SetResult(parsed_info.GetFalseResult());

		if (DEBUG) {
			if (parsed_info.IsSemanticsInvalid()) {
				cout << "Query - Semantically Invalid" << endl;
			}
			else {
				cout << "Query - Syntactically Invalid" << endl;
			}
		}
	}
	else {
		// loop: check whats in Query info
		if (DEBUG) {
			parsed_info.PrintEntityMap();
			parsed_info.PrintOutputList();
			parsed_info.PrintStMap();
			parsed_info.PrintPatternMap();
			parsed_info.PrintWithMap();
		}

		// Toggle optimization switch
		if (OPTIMIZED_EVALUATION) {
			if (DEBUG) {
				cout << "Using optimized evaluator" << endl;
			}

			result = PQLOptimizedEvaluator().Evaluate(parsed_info);
		}
		else {
			if (DEBUG) {
				cout << "Using basic evaluator" << endl;
			}

			result = PQLEvaluator().Evaluate(parsed_info);
		}

		// Clear PKB's cache
		PKB().ClearCache();
	}

	final_result = projector.Project(result);
	return final_result;
}