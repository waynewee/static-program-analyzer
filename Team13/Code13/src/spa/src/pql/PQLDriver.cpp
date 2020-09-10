#include "PQLDriver.h"

#include <iostream>

#include "PQLEvaluator.h"
#include "PQLParser.h"
#include "PQLProjector.h"
#include "QueryInfo.h"
#include "QueryResult.h"

void PQLDriver::query(string queryString) {
	PQLParser parser;
	PQLEvaluator evaluator;
	PQLProjector projector;

	/*
	string user_input;
	cout << "           Enter your input : \n";
	getline(cin, user_input);
	// cout << "Your input is : " << user_input << endl;
	return user_input;
	*/


	//QueryInfo parsedInfo;
	
	//parsedInfo = parser.parse(queryString);

	//QueryResult result = evaluator.evaluate(parsedInfo);

	//string finalResult = projector.project(result);
	//cout << "Final Result: \"" << finalResult << "\"" << std::endl;
}