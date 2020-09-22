#include <fstream>
#include <string>
#include <stdexcept>

#include <PKB.h>
#include "TestWrapper.h"
#include "frontend/SimpleParser.h"
#include "frontend/Tokenizer.h"
#include "frontend/FileReader.h"
#include "pql/PQLDriver.h"
#include "pkb/DesignExtractor.h"
#include "pkb/PatternManager.h"

using namespace std;

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
void TestWrapper::parse(string filename) {
	try {
		FileReader fileReader(filename);

		string input = fileReader.ReadFile();

		Tokenizer tokenizer(input);

		TOKEN_LIST tokenList = tokenizer.GetTokenList();

		SimpleParser parser = SimpleParser();

		TestWrapper::pkb->SetASTRoot(parser.Parse(tokenList));

		ExtractData(pkb->GetDataManager(), pkb->GetASTRoot());
		ExtractFollows(pkb->GetRelationManager(), pkb->GetASTRoot());
		ExtractParent(pkb->GetRelationManager(), pkb->GetASTRoot());
		// ExtractModifies(pkb->GetRelationManager(), pkb->GetASTRoot());
		// ExtractUses(pkb->GetRelationManager(), pkb->GetASTRoot()); 
	}
	catch (logic_error& e) {
		cout << e.what() << endl;
	}

}

// method to evaluating a query
void TestWrapper::Evaluate(string query, list<string>& results){
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	PQLDriver* main = new PQLDriver();
	string evaluated_result = main->Query(query);

	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
	results.push_back(evaluated_result);
}
