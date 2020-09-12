
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
stmt s; Select s
*/

TEST_CASE("Test 1") {

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
-------------- ALL FOLLOWS TESTCASES ----------------

1 such that and 1 unknown synonym, only constant parameters.

1st argument constant
stmt s1, s2; Select s1 such that Follows (6, s1)

*/

TEST_CASE("Test 2") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1, s2; Select s1 such that Follows (6, s1)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("s1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s1"] = "stmt";
	expected_var_map["s2"] = "stmt";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("s1");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*
1 such that and 1 unknown synonym, only constant parameters.

1st argument constant
if i1; Select i1 such that Follows* (6, i1)

*/

TEST_CASE("Test 3") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if i1; Select i1 such that Follows* (6, i1)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("i1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["i1"] = "if";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("i1");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*
1 such that and 1 unknown synonym, only constant parameters.

2nd argument constant
read r1; Select r1 such that Follows (r1, 6)

*/

TEST_CASE("Test 4") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "read r1; Select r1 such that Follows (r1, 6)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("r1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["r1"] = "read";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("r1");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*
1 such that and 1 unknown synonym, only constant parameters.

2nd argument constant
assign a1; Select a1 such that Follows* (a1, 6)

*/

TEST_CASE("Test 5") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a1; Select a1 such that Follows* (a1, 6)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("a1");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

Both arguments constant (no unknown synonym)
print p; Select p such that Follows (6, 7)

*/

TEST_CASE("Test 6") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "print p; Select p such that Follows (6, 7)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "print";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("7");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

Both arguments constant (no unknown synonym)
call c; Select c such that Follows* (6, 7)

*/

TEST_CASE("Test 7") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; Select c such that Follows* (6, 7)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("7");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

1 such that and 2 unknown synonyms
while w1, w2; Select w1 such that Follows (w1, w2)

*/

TEST_CASE("Test 8") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "while w1, w2; Select w1 such that Follows (w1, w2)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("w1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["w1"] = "while";
	expected_var_map["w2"] = "while";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("w1");
	arguments.push_back("w2");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

1 such that and 2 unknown synonyms
stmt s1; if i1; Select s1 such that Follows* (s1, i1)

*/

TEST_CASE("Test 9") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1; if i1; Select s1 such that Follows* (s1, i1)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("s1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s1"] = "stmt";
	expected_var_map["i1"] = "if";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("s1");
	arguments.push_back("i1");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*
------------------- PARENT CLAUSES -----------------
*/

/*

1st argument constant
stmt s1; Select s1 such that Parent (3, s1)

*/

TEST_CASE("Test 10") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1; Select s1 such that Parent (3, s1)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("s1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s1"] = "stmt";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("3");
	arguments.push_back("s1");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

1st argument constant
read r1; Select r1 such that Parent* (3, r1)

*/

TEST_CASE("Test 11") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "read r1; Select r1 such that Parent* (3, r1)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("r1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["r1"] = "read";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("3");
	arguments.push_back("r1");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

2nd argument constant
print p; Select p such that Parent (p, 4)

*/

TEST_CASE("Test 12") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "print p; Select p such that Parent (p, 4)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "print";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("4");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

2nd argument constant
if i1; Select i1 such that Parent* (i1, 4)

*/

TEST_CASE("Test 13") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if i1; Select i1 such that Parent* (i1, 4)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("i1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["i1"] = "if";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("i1");
	arguments.push_back("4");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

Both arguments constant (no unknown synonym)
assign a1; Select a1 such that Parent (5, 6)


*/

TEST_CASE("Test 14") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a1; Select a1 such that Parent (5, 6)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("5");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

Both arguments constant (no unknown synonym)
call c; Select c such that Parent* (5, 6)


*/

TEST_CASE("Test 15") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; Select c such that Parent* (5, 6)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("5");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

1 such that and 2 unknown synonyms
assign a1, a2; Select a1 such that Parent (a1, a2)


*/

TEST_CASE("Test 16") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a1, a2; Select a1 such that Parent (a1, a2)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	expected_var_map["a2"] = "assign";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("a1");
	arguments.push_back("a2");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*

1 such that and 2 unknown synonyms
call c1; if i1; Select c1 such that Parent* (c1, i1)

*/

TEST_CASE("Test 17") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c1; if i1; Select c1 such that Parent* (c1, i1)";

	query_info_actual = pql_parser.parse(pql_query);

	query_info_expected.setOutputVar("c1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c1"] = "call";
	expected_var_map["i1"] = "if";
	query_info_expected.setVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("c1");
	arguments.push_back("i1");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.setRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	require(are_similar);
}

/*
One clause, two different declaration types
*/
TEST_CASE("nth Test") {
	
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

