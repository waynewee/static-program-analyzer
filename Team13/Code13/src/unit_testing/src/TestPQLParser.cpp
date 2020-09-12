
#include "TNode.h"
#include "pql\PQLParser.h"
#include "pql\QuerySyntaxValidator.h"
#include "pql\QueryInfo.h"
#include "catch.hpp"
using namespace std;
#include <map>
#include <iostream>

void require(bool b) {
	REQUIRE(b);
}


/*
Returns true if both Query Infos are equivalent, else false
*/

bool compareQueryInfo(QueryInfo actual, QueryInfo expected) {
	bool result = true;
	if (actual.getOutputVar().compare(expected.getOutputVar()) != 0) {
		// if the output variables are different
		// cout << "output var not equal" << endl;
		result = false;
	}

	if (actual.getVarMap() != expected.getVarMap()) {
		// cout << "maps are not equal" << endl;
		result = false;
	}

	if (actual.getRelRefMap() != expected.getRelRefMap()) {
		result = false;
	}	

	return result;
}

/*
--------------------- VALID TESTCASES -------------------
/*
Without clause
*/

TEST_CASE("1st Test") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; Select s";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("s");

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*
One clause, two different declaration types
*/
TEST_CASE("2nd Test") {
	
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; procedure p; Select p such that Modifies (p , \"x\")";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s"] = "stmt";
	expected_var_map["p"] = "procedure";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("\"x\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesP"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
	
}


TEST_CASE("3rd Test") {

	//TNode T;



	require(1 == 1);
}

