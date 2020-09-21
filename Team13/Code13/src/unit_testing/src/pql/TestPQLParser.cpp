#include "TNode.h"
#include "pql/PQLParser.h"
#include "pql/QuerySyntaxValidator.h"
#include "pql/QueryInfo.h"
#include "catch.hpp"
using namespace std;
#include <map>
#include <iostream>

void requireBool(bool b) {
	REQUIRE(b);
}

TEST_CASE("Multiple Clause testcase") {
	PQLParser* pql_parser = new PQLParser();
	QueryInfo* query_info_actual = new QueryInfo();
	QueryInfo* query_info_expected = new QueryInfo();

	string pql_query = "assign a; variable v; Select a such that Uses (a, v) pattern a (v, _) pattern a (_ , _\"x + y\"_)";

	query_info_actual = pql_parser->Parse(pql_query);

	query_info_actual->PrintRelRefMap();
	query_info_actual->PrintVarMap();
	query_info_actual->PrintOutputVar();

	REQUIRE(1 == 1);

}

/*INVALID TESTCASES*/

TEST_CASE("Invalid testcase : Modifies first argument underscore ") {
	PQLParser* pql_parser = new PQLParser();
	QueryInfo* query_info_actual = new QueryInfo();
	QueryInfo* query_info_expected = new QueryInfo();

	string pql_query = "stmt s; Select s such that Modifies (_ ,\"x\")";

	query_info_actual = pql_parser->Parse(pql_query);

	//query_info_actual->PrintRelRefMap();
	//query_info_actual->PrintVarMap();
	//query_info_actual->PrintOutputVar();

	REQUIRE(!query_info_actual->IsQueryInfoValid());

}