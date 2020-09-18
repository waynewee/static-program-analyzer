#include "PQLDriver.h"

#include <iostream>

#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"

string PQLDriver::query(string queryString) {
	/*PQLParser parser;
	PQLEvaluator evaluator;
	PQLProjector projector;

	string finalResult = *(new string());

	cout << "Query: " << queryString << endl;
	QueryInfo parsedInfo = parser.parse(queryString);
	
	if (!parsedInfo.isQueryInfoValid()) {
		// Invalid query
		cout << "Query is invalid." << endl;
		finalResult.assign("");
		return finalResult;
	}

	// loop: check whats in query info
	parsedInfo.printOutputVar();
	parsedInfo.printRelRefMap();
	parsedInfo.printVarMap();
	
	QueryResult result = evaluator.evaluate(parsedInfo);
	if (result.isEmpty()) {
		// Empty result
		cout << "Result is empty." << endl;
		finalResult.assign("");
		return finalResult;
	}

	// loop: check whats in query result

	finalResult = projector.project(result);
	if (finalResult.empty()) {
		cout << "Projecting result has errors. 'finalResult' should not be empty as it is already caught." << endl;
	}

	cout << "Final Result: \"" << finalResult << "\"" << std::endl;
	cout << "________________________________________________________" << endl;
	return finalResult;*/ return "";
}