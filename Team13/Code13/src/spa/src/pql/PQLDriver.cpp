#include "PQLDriver.h"

#include <iostream>

#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"

STRING_PTR PQLDriver::query(STRING_PTR query_string) {
	PQLParser* parser = new PQLParser();
	PQLEvaluator* evaluator = new PQLEvaluator();
	PQLProjector* projector = new PQLProjector();

	STRING_PTR final_result = new STRING();

	if (DEBUG) {
		cout << "PQLDriver - Query: " << query_string << endl;
	}
	
	// PUTTING & AND * HERE BECAUSE PARSER NOT YET REFACTOR. 
	// ONCE DONE, WILL CHANGE BACK.
	QueryInfo* parsedInfo = parser->parse(*query_string);
	
	if (!parsedInfo->IsQueryInfoValid()) {
		// Invalid query
		if (DEBUG) {
			cout << "PQLDriver: Query is invalid." << endl;
		}
		
		final_result->assign("");
		return final_result;
	}

	// loop: check whats in query info
	if (DEBUG) {
		cout << "-----------------PQLDriver: PRINTING QUERY INFO-------------------" << endl;
		parsedInfo->PrintOutputVar();
		parsedInfo->PrintRelRefMap();
		parsedInfo->PrintVarMap();
	}
	
	QueryResult* result = evaluator->Evaluate(parsedInfo);
	if (result->isEmpty()) {
		// Empty result
		if (DEBUG) {
			cout << "PQLDriver: Result is empty." << endl;
		}

		final_result->assign("");
		return final_result;
	}

	// loop: check whats in query result

	final_result = projector->project(result);
	if (final_result->empty()) {
		// error
		if (DEBUG) {
			cout << "PQLDriver: Projecting result has errors. 'final_result' should not be empty as it is already caught." << endl;
		}
	}

	if (DEBUG) {
		cout << "PQLDriver: Final Result = \"" << final_result << "\"" << std::endl;
		cout << "________________________________________________________" << endl;
	}
	return final_result;
}