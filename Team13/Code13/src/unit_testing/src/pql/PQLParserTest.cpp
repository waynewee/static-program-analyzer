#include "pql/PQLParser.h"
#include "pql/QueryInfo.h"
#include "pql/PQLCustomTypes.h"
#include "catch.hpp"
using namespace std;
#include <map>
#include <iostream>

/*
Returns true if both Query Infos are equivalent, else false
*/

bool compareQueryInfo(QueryInfo actual, QueryInfo expected) {
	bool result = true;
	if (actual.GetOutputList() != expected.GetOutputList()) {
		// if the output variables are different
		cout << "output list not equal" << endl;
		result = false;
		return result;
	}

	if (actual.GetEntityMap() != expected.GetEntityMap()) {
		// cout << "maps are not equal" << endl;
		result = false;
		return result;
	}

	if (actual.GetPatternMap() != expected.GetPatternMap()) {
		result = false;
		return result;
	}

	if (actual.GetStMap() != expected.GetStMap()) {
		result = false;
		return result;
	}

	if (actual.GetWithMap() != expected.GetWithMap()) {
		result = false;
		return result;
	}

	return result;
}


// --------------------- VALID TESTCASES -------------------

// Without clause
// stmt s; Select s

TEST_CASE("Without clause") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; Select s";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// 1st argument constant
// if i1; Select i1 such that Follows* (6, i1)

TEST_CASE("such that clause, Follows*, first argument constant") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "if i1; Select i1 such that Follows* (6, i1)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("i1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["i1"] = TYPE_DESIGN_ENTITY_IF;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("6");
	st_single_result.push_back("i1");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS_T] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// 2nd argument constant
// read r1; Select r1 such that Parent(r1, 6)

TEST_CASE("such that clause, Parent, second argument constant") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "read r1; Select r1 such that Parent(r1, 6)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("r1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["r1"] = TYPE_DESIGN_ENTITY_READ;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("r1");
	st_single_result.push_back("6");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_PARENT] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// 1 such that and 2 unknown synonyms
// stmt s1; if i1; Select s1 such that Follows (s1, i1)

TEST_CASE("such that clause, Follows, both unknown synonyms") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1; if i1; Select s1 such that Follows (s1, i1)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s1"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["i1"] = TYPE_DESIGN_ENTITY_IF;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s1");
	st_single_result.push_back("i1");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for ModifiesS
// call c; variable v; Select s1 such that Modifies (c, v)

TEST_CASE("such that clause, testing for ModifiesS") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "call c; variable v; Select c such that Modifies (c, v)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("c");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["c"] = TYPE_DESIGN_ENTITY_CALL;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("c");
	st_single_result.push_back("v");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_MODIFIES_S] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for ModifiesP
// procedure p; Select p such that Modifies (p, "x")

TEST_CASE("such that clause, testing for ModifiesP") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Modifies (p, \"x\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PROCEDURE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("p");
	st_single_result.push_back("\"x\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_MODIFIES_P] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for UsesS
// print p; Select p such that Uses (p, “y”)

TEST_CASE("such that clause, testing for UsesS") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "print p; Select p such that Uses (p, \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PRINT;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("p");
	st_single_result.push_back("\"y\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_S] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for UsesS
// variable v; Select v such that Uses ("x", "y")

TEST_CASE("such that clause, testing for UsesP") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "variable v; Select v such that Uses(\"x\", \"y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("v");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("\"x\"");
	st_single_result.push_back("\"y\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_P] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for Next*
// stmt s; prog_line n; Select s such that Next* (s, n)

TEST_CASE("such that clause, testing for Next*") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; prog_line n; Select s such that Next* (s, n)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["n"] = TYPE_DESIGN_ENTITY_PROG_LINE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s");
	st_single_result.push_back("n");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_NEXT_T] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for Next*
// stmt s1, s2; Select s1 such that Affects (s1, s2)

TEST_CASE("such that clause, testing for Affects") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s1, s2; Select s1 such that Affects (s1, s2)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s1");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s1"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["s2"] = TYPE_DESIGN_ENTITY_STMT;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s1");
	st_single_result.push_back("s2");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_AFFECTS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Testing for Calls
// procedure p; Select p such that Calls (_, _)

TEST_CASE("such that clause, testing for Calls both empty") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "procedure p; Select p such that Calls (_, _)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("p");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["p"] = TYPE_DESIGN_ENTITY_PROCEDURE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("_");
	st_single_result.push_back("_");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_CALLS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Test for attrRef in result clause
// assign a; Select a.stmt# such that Follows (a, 10)

TEST_CASE("attrRef in result clause. a.stmt#") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; Select a.stmt# such that Follows (a, 10)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("a.stmt#");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("a");
	st_single_result.push_back("10");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Test for attrRef in result clause
// variable v; Select v.varName such that Uses (v, "z")

TEST_CASE("attrRef in result clause. v.varName") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "variable v; Select v.varName such that Uses (v, \"z\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("v.varName");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("v");
	st_single_result.push_back("\"z\"");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_P] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Tuple in result clause :
// stmt s, s2, s3; variable v; Select <  s , s2 >  such that Follows (s, s2)

TEST_CASE("one tuple") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s, s2; variable v; Select <  s , s2 >  such that Follows (s, s2)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");
	expected_output_list.push_back("s2");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["s2"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s");
	st_single_result.push_back("s2");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_FOLLOWS] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Tuple with attrRef
// stmt s; variable v; Select <s.stmt#, v.varName> such that Uses(s, v)

TEST_CASE("one tuple with attr ref") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s; variable v; Select <s.stmt#, v.varName> such that Uses(s, v)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s.stmt#");
	expected_output_list.push_back("v.varName");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["v"] = TYPE_DESIGN_ENTITY_VARIABLE;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result;
	STRING_LIST st_single_result;
	st_single_result.push_back("s");
	st_single_result.push_back("v");
	st_result.push_back(st_single_result);
	expected_st_map[TYPE_COND_USES_S] = st_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}


// Test for MULTIPLE CLAUSES
// Big tuple, Multiple clauses

// Tuple in result clause :
// stmt s, s2; assign a, a2, a3; 
// Select <s, s2, a, a2, a3> such that Follows (s, 10) and Follows* (5, s2) and Parent*( a, a2) such that Parent(2, a3)

TEST_CASE("big tuple, multiple clauses, similar relref and different relref") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "stmt s, s2; assign a, a2, a3; Select <s, s2, a, a2, a3> such that Follows (s, 10) and Follows(5, s2) and Parent*( a, a2) such that Parent(2, a3)";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("s");
	expected_output_list.push_back("s2");
	expected_output_list.push_back("a");
	expected_output_list.push_back("a2");
	expected_output_list.push_back("a3");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["s"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["s2"] = TYPE_DESIGN_ENTITY_STMT;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;
	expected_entity_map["a2"] = TYPE_DESIGN_ENTITY_ASSIGN;
	expected_entity_map["a3"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_st_map;
	STRINGLIST_LIST st_result_one;
	STRING_LIST st_single_result_one;
	st_single_result_one.push_back("s");
	st_single_result_one.push_back("10");

	STRING_LIST st_single_result_two;
	st_single_result_two.push_back("5");
	st_single_result_two.push_back("s2");

	STRINGLIST_LIST st_result_two;
	STRING_LIST st_single_result_three;
	st_single_result_three.push_back("a");
	st_single_result_three.push_back("a2");

	STRINGLIST_LIST st_result_three;
	STRING_LIST st_single_result_four;
	st_single_result_four.push_back("2");
	st_single_result_four.push_back("a3");


	st_result_one.push_back(st_single_result_one);
	st_result_one.push_back(st_single_result_two);
	
	st_result_two.push_back(st_single_result_three);
	st_result_three.push_back(st_single_result_four);

	expected_st_map[TYPE_COND_FOLLOWS] = st_result_one;
	expected_st_map[TYPE_COND_PARENT_T] = st_result_two;
	expected_st_map[TYPE_COND_PARENT] = st_result_three;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetStMap(expected_st_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}

// Test for pattern clauses :

TEST_CASE("simple pattern clause, assign type") {
	PQLParser pql_parser;
	QueryInfo query_info_actual;
	QueryInfo query_info_expected;

	string pql_query = "assign a; Select a pattern a (_ , \"x + y\")";

	query_info_actual = pql_parser.Parse(pql_query);

	STRING_LIST expected_output_list;
	expected_output_list.push_back("a");

	STRING_STRING_MAP expected_entity_map;
	expected_entity_map["a"] = TYPE_DESIGN_ENTITY_ASSIGN;

	STRING_STRINGLISTLIST_MAP expected_pattern_map;
	STRINGLIST_LIST pattern_result;
	STRING_LIST pattern_single_result;
	pattern_single_result.push_back("_");
	pattern_single_result.push_back("\"x + y\"");
	pattern_single_result.push_back("a");
	pattern_result.push_back(pattern_single_result);
	
	expected_pattern_map[TYPE_COND_PATTERN_F] = pattern_result;

	query_info_expected.SetOutputList(expected_output_list);
	query_info_expected.SetEntityMap(expected_entity_map);
	query_info_expected.SetPatternMap(expected_pattern_map);

	bool are_similar = compareQueryInfo(query_info_actual, query_info_expected);

	REQUIRE(are_similar);
}