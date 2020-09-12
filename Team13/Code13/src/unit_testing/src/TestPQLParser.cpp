
#include "TNode.h"
#include "pql\PQLParser.h"
#include "pql\QuerySyntaxValidator.h"
#include "pql\QueryInfo.h"
#include "catch.hpp"
using namespace std;

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
		result = false;
	}

	return result;
}

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


TEST_CASE("2nd Test") {

	//TNode T;



	require(1 == 1);
}

