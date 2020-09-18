#include "PQLDriver.h"

#include <iostream>

#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"

STRING_PTR PQLDriver::query(STRING_PTR queryString) {
	PQLParser* parser = new PQLParser();
	PQLEvaluator* evaluator = new PQLEvaluator();
	PQLProjector* projector = new PQLProjector();

	STRING_PTR finalResult = new STRING();

	if (DEBUG) {
		cout << "PQLDriver - Query: " << queryString << endl;
	}
	
	// PUTTING & AND * HERE BECAUSE PARSER NOT YET REFACTOR. 
	// ONCE DONE, WILL CHANGE BACK.
	QueryInfo* parsedInfo = &(parser->parse(*queryString));
	
	if (!parsedInfo->isQueryInfoValid()) {
		// Invalid query
		if (DEBUG) {
			cout << "PQLDriver: Query is invalid." << endl;
		}
		
		finalResult->assign("");
		return finalResult;
	}

	// loop: check whats in query info
	if (DEBUG) {
		cout << "-----------------PQLDriver: PRINTING QUERY INFO-------------------" << endl;
		parsedInfo->printOutputVar();
		parsedInfo->printRelRefMap();
		parsedInfo->printVarMap();
	}
	
	QueryResult* result = evaluator->Evaluate(parsedInfo);
	if (result->isEmpty()) {
		// Empty result
		if (DEBUG) {
			cout << "PQLDriver: Result is empty." << endl;
		}

		finalResult->assign("");
		return finalResult;
	}

	// loop: check whats in query result

	finalResult = projector->project(result);
	if (finalResult->empty()) {
		// error
		if (DEBUG) {
			cout << "PQLDriver: Projecting result has errors. 'finalResult' should not be empty as it is already caught." << endl;
		}
	}

	if (DEBUG) {
		cout << "PQLDriver: Final Result = \"" << finalResult << "\"" << std::endl;
		cout << "________________________________________________________" << endl;
	}
	return finalResult;
}