#include "PQLDriver.h"
#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"
#include <iostream>

STRING PQLDriver::Query(STRING query_string) {
	PQLParser parser = PQLParser();
	PQLEvaluator evaluator = PQLEvaluator();
	PQLProjector projector = PQLProjector();

	STRING final_result = "";

	cout << "Query: " << query_string << endl;
	QueryInfo parsed_info = parser.parse(query_string);

	if (!parsed_info.IsQueryInfoValid()) {
		// Invalid Query
		cout << "Query is invalid." << endl;
		final_result.assign("");
		return final_result;
	}

	// loop: check whats in Query info
	parsed_info.PrintOutputVar();
	parsed_info.PrintRelRefMap();
	parsed_info.PrintVarMap();

	QueryResult result = evaluator.Evaluate(parsed_info);
	if (result.IsEmpty()) {
		// Empty result
		cout << "Result is empty." << endl;
		final_result.assign("");
		return final_result;
	}

	// loop: check whats in Query result

	final_result = projector.Project(result);
	if (final_result.empty()) {
		cout << "Projecting result has errors. 'final_result' should not be empty as it is already caught." << endl;
	}

	cout << "Final Result: \"" << final_result << "\"" << std::endl;
	cout << "________________________________________________________" << endl;
	return final_result;
}
