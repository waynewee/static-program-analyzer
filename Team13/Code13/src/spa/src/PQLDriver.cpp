#include <string>
using namespace std;

#include "PQLDriver.h"
#include "PQLParser.h"
#include "PQLEvaluator.h"
//#include "PQLProjector.h"

#include "QueryInfo.h"
#include "QueryResult.h"

void PQLDriver::query(string queryString) {
	PQLParser parser;
	PQLEvaluator evaluator;
	//PQLProjector projector;


	//QueryInfo parsedInfo = parser.parse(queryString);

	QueryResult result = evaluator.evaluate(parsedInfo);

	//projector.project(result);
}