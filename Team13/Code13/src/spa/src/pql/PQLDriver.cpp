#include "PQLDriver.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"
#include <iostream>

STRING_SET PQLDriver::Query(string query_string) {
	PQLParser parser = PQLParser();
	PQLEvaluator evaluator = PQLEvaluator();
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
				cout << "QUERY IS INVALID DUE TO SEMANTICS, NOT SYNTAX" << endl;
			}
			else {
				cout << "NOTHING TO DO WITH JUST SEMANTICS" << endl;
			}
		}
	}
	else {
		result = evaluator.Evaluate(parsed_info);

		// loop: check whats in Query info
		if (DEBUG) {
			parsed_info.PrintEntityMap();
			parsed_info.PrintOutputList();
			parsed_info.PrintStMap();
			parsed_info.PrintPatternMap();
			parsed_info.PrintWithMap();
		}
	}

	// loop: check whats in Query result

	final_result = projector.Project(result);

	return final_result;
}