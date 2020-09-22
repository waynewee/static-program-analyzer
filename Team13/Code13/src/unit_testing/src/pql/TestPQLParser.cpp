
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

/*
Returns true if both Query Infos are equivalent, else false
*/

bool compareQueryInfo(QueryInfo actual, QueryInfo expected) {
	bool result = true;
	if (actual.GetOutputVar().compare(expected.GetOutputVar()) != 0) {
		// if the output variables are different
		// cout << "output var not equal" << endl;
		result = false;
	}

	if (actual.GetVarMap() != expected.GetVarMap()) {
		// cout << "maps are not equal" << endl;
		result = false;
	}

	if (actual.GetRelRefMap() != expected.GetRelRefMap()) {
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s");

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s1"] = "stmt";
	expected_var_map["s2"] = "stmt";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("s1");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("i1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["i1"] = "if";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("i1");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("r1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["r1"] = "read";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("r1");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("a1");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "print";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("7");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("6");
	arguments.push_back("7");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("w1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["w1"] = "while";
	expected_var_map["w2"] = "while";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("w1");
	arguments.push_back("w2");

	all_arguments.push_back(arguments);

	expected_relRef_map["Follows"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s1"] = "stmt";
	expected_var_map["i1"] = "if";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("s1");
	arguments.push_back("i1");

	all_arguments.push_back(arguments);

	expected_relRef_map["FollowsT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s1"] = "stmt";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("3");
	arguments.push_back("s1");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("r1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["r1"] = "read";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("3");
	arguments.push_back("r1");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "print";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("4");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("i1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["i1"] = "if";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("i1");
	arguments.push_back("4");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("5");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("5");
	arguments.push_back("6");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	expected_var_map["a2"] = "assign";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("a1");
	arguments.push_back("a2");

	all_arguments.push_back(arguments);

	expected_relRef_map["Parent"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
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

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("c1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c1"] = "call";
	expected_var_map["i1"] = "if";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("c1");
	arguments.push_back("i1");

	all_arguments.push_back(arguments);

	expected_relRef_map["ParentT"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument : stmt vs procedure
while w; Select w such that Uses (w ,"x")

*/

TEST_CASE("Test 18") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "while w; Select w such that Uses (w ,\"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("w");

	unordered_map<string, string> expected_var_map;
	expected_var_map["w"] = "while";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("w");
	arguments.push_back("\"x\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument : stmt vs procedure
procedure p; Select p such that Uses (p ,"x")

*/

TEST_CASE("Test 19") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Uses (p ,\"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("\"x\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument INTEGER vs “IDENT”
assign a1; Select a1 such that Uses (5, “y”)

*/

TEST_CASE("Test 20") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a1; Select a1 such that Uses (5, \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("5");
	arguments.push_back("\"y\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument INTEGER vs “IDENT”
call c; Select c such that Uses ("x", "y")

*/

TEST_CASE("Test 21") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; Select c such that Uses (\"x\", \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("\"x\"");
	arguments.push_back("\"y\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Second argument declared as procedure
procedure p1, p2; Select p1 such that Uses (p1, p2)

*/

TEST_CASE("Test 22") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p1, p2; Select p1 such that Uses (p1, p2)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p1"] = "procedure";
	expected_var_map["p2"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p1");
	arguments.push_back("p2");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Second argument declared as procedure
stmt s; procedure p; Select s such that Uses (s, p)

*/

TEST_CASE("Test 23") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; procedure p; Select s such that Uses (s, p)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s"] = "stmt";
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("s");
	arguments.push_back("p");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Second argument declared as procedure
if i1; Select i1 such that Uses (i1, _)

*/

TEST_CASE("Test 24") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if i1; Select i1 such that Uses (i1, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("i1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["i1"] = "if";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("i1");
	arguments.push_back("_");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Second argument declared as procedure
procedure p; Select p such that Uses (p, _)

*/

TEST_CASE("Test 25") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Uses (p, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("_");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/* -------------- MODIFIES TEST CASES -------------------*/

/*

First argument stmt vs procedure
stmt s; Select s such that Modifies (s ,"x")

*/

TEST_CASE("Test 26") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; Select s such that Modifies (s ,\"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s"] = "stmt";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("s");
	arguments.push_back("\"x\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument stmt vs procedure
procedure p; Select p such that Modifies (p ,"x")

*/

TEST_CASE("Test 27") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Modifies (p ,\"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("\"x\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument INTEGER vs “IDENT”
assign a1; Select a1 such that Modifies (5, "y")

*/

TEST_CASE("Test 28") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a1; Select a1 such that Modifies (5, \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("5");
	arguments.push_back("\"y\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

First argument INTEGER vs “IDENT”
call c; Select c such that Modifies ("x", "y")

*/

TEST_CASE("Test 29") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; Select c such that Modifies (\"x\", \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("\"x\"");
	arguments.push_back("\"y\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}


/*

Second argument declared as procedure
procedure p1, p2; Select p1 such that Modifies (p1, p2)

*/

TEST_CASE("Test 30") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p1, p2; Select p1 such that Modifies (p1, p2)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p1"] = "procedure";
	expected_var_map["p2"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p1");
	arguments.push_back("p2");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Second argument declared as procedure
stmt s; procedure p; Select s such that Modifies (s, p)

*/

TEST_CASE("Test 31") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; procedure p; Select s such that Modifies (s, p)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s"] = "stmt";
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("s");
	arguments.push_back("p");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Underscore as second argument
if i1; Select i1 such that Modifies (i1, _)

*/

TEST_CASE("Test 32") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if i1; Select i1 such that Modifies (i1, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("i1");

	unordered_map<string, string> expected_var_map;
	expected_var_map["i1"] = "if";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("i1");
	arguments.push_back("_");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*

Underscore as second argument
procedure p; Select p such that Modifies (p, _)

*/

TEST_CASE("Test 33") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Modifies (p, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("_");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*
One clause, two different declaration types
*/
TEST_CASE("Test 34") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; procedure p; Select p such that Modifies (p , \"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("p");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s"] = "stmt";
	expected_var_map["p"] = "procedure";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("p");
	arguments.push_back("\"x\"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesP"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);

}

/* ------------ PATTERN TEST CASES ---------------- */

/*
First and second argument are underscores :
assign a; variable v; Select a pattern a (_, _)
*/
TEST_CASE("Test 35") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a (_, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a"] = "assign";
	expected_var_map["v"] = "variable";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("_");
	arguments.push_back("_");
	arguments.push_back("a");

	all_arguments.push_back(arguments);

	expected_relRef_map["pattern_f"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*
First argument underscore :
assign a; variable v; Select a pattern a (_, "z  * y / s + x")
*/
TEST_CASE("Test 36") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a(_, \"z * y / s + x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a"] = "assign";
	expected_var_map["v"] = "variable";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("_");
	arguments.push_back("\"z * y / s + x\"");
	arguments.push_back("a");

	all_arguments.push_back(arguments);

	expected_relRef_map["pattern_f"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*
Second argument with leading and trailing underscores :
assign a; variable v; Select a pattern a (v, _"z  * y / s + x"_)
*/
TEST_CASE("Test 37") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a (v, _\"z * y / s + x\"_)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a"] = "assign";
	expected_var_map["v"] = "variable";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("v");
	arguments.push_back("\"z * y / s + x\"");
	arguments.push_back("a");

	all_arguments.push_back(arguments);

	expected_relRef_map["pattern_p"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*
Second argument with leading and trailing underscores :
assign a; call c; variable v; Select a such that Uses (a, v) pattern a (v, _) pattern a (_ , _"x + y"_)
*/
TEST_CASE("Test 38") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; call c; variable v; Select a such that Uses (a, v) pattern a (v, _) pattern a (_ , _\"x + y\"_)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a"] = "assign";
	expected_var_map["c"] = "call";
	expected_var_map["v"] = "variable";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments_first; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments_first; // for first clause : Uses (a, v)
	arguments_first.push_back("a");
	arguments_first.push_back("v");

	all_arguments_first.push_back(arguments_first);

	vector<vector<string>> all_arguments_second; //  pattern a (v, _) pattern a (_ , _\"x + y\"_)
	vector<string> arguments_second; // for clause : pattern a (v, _)
	arguments_second.push_back("v");
	arguments_second.push_back("_");
	arguments_second.push_back("a");

	all_arguments_second.push_back(arguments_second);

	vector<vector<string>> all_arguments_third;
	vector<string> arguments_third; // for clause : pattern a (_ , _\"x + y\"_)
	arguments_third.push_back("_");
	arguments_third.push_back("\"x + y\"");
	arguments_third.push_back("a");

	all_arguments_third.push_back(arguments_third);

	expected_relRef_map["UsesS"] = all_arguments_first;
	expected_relRef_map["pattern_f"] = all_arguments_second;
	expected_relRef_map["pattern_p"] = all_arguments_third;
	
	query_info_expected.SetRelRefMap(expected_relRef_map);
	query_info_expected.PrintRelRefMap();
	query_info_actual.PrintRelRefMap();
	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/*
Second argument with leading and trailing underscores :
call c; variable v; Select c such that Uses (c, v)
*/
TEST_CASE("Test 39") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; variable v; Select c such that Uses (c, v)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("c");

	unordered_map<string, string> expected_var_map;
	expected_var_map["c"] = "call";
	expected_var_map["v"] = "variable";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("c");
	arguments.push_back("v");

	all_arguments.push_back(arguments);

	expected_relRef_map["UsesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/* -------------- TEST CASES WITH MULTIPLE SPACEBAR ----------- */
/*

First argument stmt vs procedure
stmt   s   ;   Select    s     such that         Modifies   (   s    ,  "  x  "  )

*/

TEST_CASE("Test 40") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt   s   ;   Select    s     such that         Modifies   (   s    ,  \"  x  \"  )  ";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("s");

	unordered_map<string, string> expected_var_map;
	expected_var_map["s"] = "stmt";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("s");
	arguments.push_back("\"  x  \"");

	all_arguments.push_back(arguments);

	expected_relRef_map["ModifiesS"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}

/* -------------- INVALID TEST CASES ----------------- */

/*

Modifies first argument underscore : invalid as cannot tell between ModifiesS and ModifiesP
stmt s; Select s such that Modifies (_ ,"x")

*/

TEST_CASE("Test 41") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; Select s such that Modifies (_ ,\"x\")";

	try {
		query_info_actual = pql_parser.Parse(pql_query);
	}
	catch (exception e) {

	}
	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid 2nd argument for pattern :
assign a; variable v; Select a pattern a (v, _"z")

*/

TEST_CASE("Test 42") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a (v, _\"z\")";

	try {
		query_info_actual = pql_parser.Parse(pql_query);
	}
	catch (exception e) {

	}

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid 2nd argument for pattern :
assign a; variable v; Select a pattern a (v, "z"_)

*/

TEST_CASE("Test 43") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a (v, \"z\"_)";

	try {
		query_info_actual = pql_parser.Parse(pql_query);
	}
	catch (exception e) {

	}

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid 2nd argument for pattern :
assign a; variable v; Select a pattern a (v, _"+ z  + y"_)

*/

TEST_CASE("Test 44") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a (v, _\" + z + y\"_)";

	try {
		query_info_actual = pql_parser.Parse(pql_query);
	}
	catch (exception e) {

	}

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid 2nd argument for pattern :
assign a; variable v; Select a pattern a (v, _" ) z  + y ( "_)

*/

TEST_CASE("Test 45") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; variable v; Select a pattern a (v, _\" ) z + y(\"_)";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid variable name for 2nd argument :
procedure p; Select p such that Uses (p ,"[")

*/

TEST_CASE("Test 46") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Uses (p ,\"[\")";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid synonym type :
aszign a; variable v; Select a pattern a (_, _)

*/

TEST_CASE("Test 47") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "aszign a; variable v; Select a pattern a (_, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Select clause not found :
procedure p1, p2; p1 such that Uses (p1, p2)

*/

TEST_CASE("Test 48") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p1, p2; p1 such that Uses (p1, p2)";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Such that should be only separated by single spacing :
stmt s1; Select s1 such  that Parent (3, s1)

*/

TEST_CASE("Test 49") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1; Select s1 such  that Parent (3, s1)";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid variable name :
stmt 0s; Select 0s

*/

TEST_CASE("Test 50") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt 0s; Select 0s";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Invalid variable name :
stmt [/v; Select [/v

*/

TEST_CASE("Test 51") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt [/v; Select [/v";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Comma not found in arguments :
call c1; if i1; Select c1 such that Parent* (c1 i1)

*/

TEST_CASE("Test 52") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c1; if i1; Select c1 such that Parent* (c1 i1)";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*

Synonym undeclared by user :
stmt s1; Select s such that Parent (3, s1)

*/

TEST_CASE("Test 53") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1; Select s such that Parent (3, s1)";

	query_info_actual = pql_parser.Parse(pql_query);

	bool isValid = query_info_actual.IsQueryInfoValid();

	requireBool(!isValid);
}

/*
ADDITIONAL TEST CASES
*/

/*
First argument underscore :
assign a1; variable v1; Select a1 pattern a1 (v1, _)
*/
TEST_CASE("Test ADDITIONAL") {

	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a1; variable v1; Select a1 pattern a1(v1, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	query_info_expected.SetOutputVar("a");

	unordered_map<string, string> expected_var_map;
	expected_var_map["a1"] = "assign";
	expected_var_map["v1"] = "variable";
	query_info_expected.SetVarMap(expected_var_map);

	unordered_map<string, vector<vector<string>>> expected_relRef_map;
	vector<vector<string>> all_arguments; // if multiple of same type of function e.g. multiple modifies
	vector<string> arguments; // for a single clause
	arguments.push_back("v1");
	arguments.push_back("_");
	arguments.push_back("a1");

	all_arguments.push_back(arguments);

	expected_relRef_map["pattern_f"] = all_arguments;

	query_info_expected.SetRelRefMap(expected_relRef_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	requireBool(are_similar);
}
