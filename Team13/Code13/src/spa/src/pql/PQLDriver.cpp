#include "PQLDriver.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"
#include <iostream>

STRING_SET PQLDriver::Query(STRING query_string) {
	PQLParser parser = PQLParser();
	PQLEvaluator evaluator = PQLEvaluator();
	PQLProjector projector = PQLProjector();

	STRING_SET final_result;

	if (DEBUG) {
		cout << "Query: " << query_string << endl;
	}
	QueryInfo parsed_info = parser.Parse(query_string);

	if (!parsed_info.IsQueryInfoValid()) {
		// Invalid Query
		if (DEBUG) {
			cout << "Query is invalid." << endl;
		}
		return final_result;
	}

	// loop: check whats in Query info
	/*parsed_info.PrintOutputList();
	parsed_info.PrintClausesMap();
	parsed_info.PrintEntityMap();*/

	QueryResult result = evaluator.Evaluate(parsed_info);
	//if (result.IsEmpty()) {
	//	// Empty result
	//	if (DEBUG) {
	//		cout << "Result is empty." << endl;
	//	}
	//	return final_result;
	//}

	// loop: check whats in Query result

	final_result = projector.Project(result);

	/*if (final_result.empty()) {
		cout << "Projecting result has errors. 'final_result' should not be empty as it is already caught." << endl;
	}*/

	return final_result;
}