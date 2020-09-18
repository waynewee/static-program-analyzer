#include <fstream>
#include <string>
#include <PKB.h>
#include "TestWrapper.h"

#include "frontend/Parser.h"
#include "frontend/CodeExtractor.h"
#include "testUtils/TreeTraverse.h"
#include "pql/PQLDriver.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	pkb = new PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {

	CodeExtractor codeExtractor(filename);

	std::string input = codeExtractor.extract();
	Parser parser = Parser();
	TestWrapper::pkb->SetASTRoot(parser.parse(input));
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	PQLDriver main = PQLDriver();
	string evaluatedResult = main.query(query);

	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
	results.push_back(evaluatedResult);
}
