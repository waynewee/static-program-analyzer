/*
#include "pql/PQLEvaluator.h"
<<<<<<< HEAD
#include "catch.hpp"
#include <iostream>
#include "pkb/PKB.h"
#include "pql/PQLDriver.h"

PQLEvaluator evaluator;
PKB pkb = PKB();


TEST_CASE("lazy testing") {
	PQLDriver pql = PQLDriver();
	cout << "LAZYYYYYYYYYYYYYYYYYYYY TESTINGGGGGGGGGGGGGGGGGGGGGG" << endl;
	cout << "__________________________________________________" << endl;
	pql.Query("stmt s1, s2; Select s1 such that Follows(s1, s2)");
	pql.Query("stmt s1, s2; Select s1 such that Follows*(s1, s2)");
	pql.Query("stmt s1, s2; Select s1 such that Parent(s1, s2)");
	pql.Query("stmt s1, s2; Select s1 such that Parent*(s1, s2)");
	pql.Query("stmt s1; variable v1; Select s1 such that Uses(s1, v1)");
	pql.Query("procedure p1; variable v1; Select p1 such that Uses(\"proc\", v1)");
	pql.Query("stmt s1; variable v1; Select s1 such that Modifies(s1, v1)");
	pql.Query("procedure p1; variable v1; Select v1 such that Modifies(p1, v1)");
	pql.Query("assign a1; variable v1; Select a1 pattern a1(v1, _)");
	pql.Query("stmt s1; assign a1; variable v1; Select v1 such that Follows(s1, a1) pattern a1(v1, _)");

	*/
// here above uncomment
	/*pql.Query("assign a1; variable v1; Select a1 pattern a1(v1, _\"x+x\"_)"); 
	pql.Query("stmt s1; assign a1; variable v1; Select v1 such that Follows(s1, a1) pattern a1(v1, _\"x+x\"_)");
	pql.Query("assign a1; variable v1; Select a1 pattern a1(v1, \"x+x\")");
	pql.Query("stmt s1; assign a1; variable v1; Select v1 such that Follows(s1, a1) pattern a1(v1, \"x+x\")");*/
// here below uncomment
	/*
cout << "__________________________________________________" << endl;

}

TEST_CASE("1 clause || Follows || user-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s1", TYPE_STMT });
	varMap.insert({ "s2", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s1", "s2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s1" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || user-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || wildcard-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "s"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || user-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || int-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"1", "s"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
=======
>>>>>>> acc9b56da87c9108465f84e78b936b9f99f56bec

#include "catch.hpp"

#include <iostream>

#include <pql/PQLParser.h>

PQLEvaluator evaluator;
PQLParser parser;

TEST_CASE("") {
	cout << "------------------------------------------PQLEVALUATOR TESTING------------------------------------" << endl;
}


TEST_CASE("ParseClauses | Parsing WITH constraints") {
	QueryInfo query_info = parser.Parse("stmt s; assign a; prog_line n; Select BOOLEAN with \"x\" = \"x\" and 10 = 10 and s.stmt# = 10 and a.stmt# = n");
	query_info.PrintWithMap();

	STRINGPAIR_SET constraints = STRINGPAIR_SET();
	evaluator.ParseClauses(query_info, &constraints);

	STRINGPAIR_SET expected_result = STRINGPAIR_SET();
	STRING_PAIR* p1 = new STRING_PAIR();
	p1->first = "s.stmt#";
	p1->second = "10";

	STRING_PAIR* p2 = new STRING_PAIR();
	p2->first = "a.stmt#";
	p2->second = "n";

	expected_result.insert(p1);
	expected_result.insert(p2);

	for (STRING_PAIR* expected_pair: expected_result) {
		BOOLEAN found = false;
		for (STRING_PAIR* constraint_pair : constraints) {
			if (expected_pair->first.compare(constraint_pair->first) == 0 && expected_pair->second.compare(constraint_pair->second) == 0) {
				found = true;
				break;
			}
		}

		REQUIRE(found);
	}
}

TEST_CASE("ParseClauses | Parsing ST constraints") {
	QueryInfo query_info = parser.Parse("stmt s; assign a; prog_line n; procedure p; variable v1, v2; Select BOOLEAN such that Parent (1, 2) and Follows (1, s) and Next (n, 10) and Uses (s, v1) and Uses (p, v1) and Modifies (n, v2) and Modifies (\"s\", \"x\")");
	query_info.PrintStMap();

	STRINGSET_STRINGLISTSET_MAP synonyms_map = STRINGSET_STRINGLISTSET_MAP();
	evaluator.ParseClauses(query_info, &synonyms_map);

	STRINGSET_STRINGLISTSET_MAP expected_result = STRINGSET_STRINGLISTSET_MAP();
	STRING_SET* k1 = new STRING_SET();
	k1->insert("s"); k1->insert("v1"); k1->insert("p");

	STRINGLIST_SET v1 = STRINGLIST_SET();
	STRING_LIST c = STRING_LIST();
	c.push_back("Follows"); c.push_back("1"); c.push_back("s");
	v1.insert(new STRING_LIST(c));
	c.clear();
	c.push_back("UsesS"); c.push_back("s"); c.push_back("v1");
	v1.insert(new STRING_LIST(c));
	c.clear();
	c.push_back("UsesP"); c.push_back("p"); c.push_back("v1");
	v1.insert(new STRING_LIST(c));
	c.clear();
	
	STRING_SET* k2 = new STRING_SET();
	k2->insert("n"); k2->insert("v2");

	STRINGLIST_SET v2 = STRINGLIST_SET();
	c.push_back("Next"); c.push_back("n"); c.push_back("10");
	v2.insert(new STRING_LIST(c));
	c.clear();
	c.push_back("Modifies"); c.push_back("n"); c.push_back("v2");
	v2.insert(new STRING_LIST(c));

	expected_result.insert({ k1, v1 });
	expected_result.insert({ k2, v2 });

	for (auto f = expected_result.cbegin(); f != expected_result.cend(); f++) {
		BOOLEAN found_key = false;
		for (auto s = synonyms_map.cbegin(); s != synonyms_map.cend(); s++) {
			if (*((*f).first) == *((*s).first)) {
				BOOLEAN found_value = false;
				for (STRING_LIST* expected_clause : (*f).second) {
					for (STRING_LIST* clause : (*s).second) {
						if (*clause == *expected_clause) {
							found_value = true;
						}
					}

					REQUIRE(found_value);
				}


				found_key = true;
				break;
			}
		}

		REQUIRE(found_key);
	}
}

TEST_CASE("ParseClauses | Parsing pattern constraints") {
	QueryInfo query_info = parser.Parse("assign a1, a2; variable v1, v2; Select BOOLEAN pattern a1 (v1, _) and a1 (v1, \"x\") and a1 (v1, _\"x\"_) and a2 (_, _\"x\"_) and a2 (_, _) and a2 (_, \"x\")");
	query_info.PrintPatternMap();

	STRINGSET_STRINGLISTSET_MAP synonyms_map = STRINGSET_STRINGLISTSET_MAP();
	evaluator.ParseClauses(query_info, &synonyms_map);

	STRINGSET_STRINGLISTSET_MAP expected_result = STRINGSET_STRINGLISTSET_MAP();
	STRING_SET* k1 = new STRING_SET();
	k1->insert("a1"); k1->insert("v1");

	STRINGLIST_SET v1 = STRINGLIST_SET();
	STRING_LIST c = STRING_LIST();
	c.push_back("pattern_f"); c.push_back("v1"); c.push_back("_"); c.push_back("a1");
	v1.insert(new STRING_LIST(c));
	c.clear();
	c.push_back("pattern_f"); c.push_back("v1"); c.push_back("\"x\""); c.push_back("a1");
	v1.insert(new STRING_LIST(c));
	c.clear();
	c.push_back("pattern_p"); c.push_back("v1"); c.push_back("\"x\""); c.push_back("a1");
	v1.insert(new STRING_LIST(c));
	c.clear();

	STRING_SET* k2 = new STRING_SET();
	k2->insert("a2");

	STRINGLIST_SET v2 = STRINGLIST_SET();
	c.push_back("pattern_f"); c.push_back("_"); c.push_back("_");  c.push_back("a2");
	v2.insert(new STRING_LIST(c));
	c.clear();
	c.push_back("pattern_p"); c.push_back("_"); c.push_back("\"x\"");  c.push_back("a2");
	v2.insert(new STRING_LIST(c));

	expected_result.insert({ k1, v1 });
	expected_result.insert({ k2, v2 });

	for (auto f = expected_result.cbegin(); f != expected_result.cend(); f++) {
		BOOLEAN found_key = false;
		for (auto s = synonyms_map.cbegin(); s != synonyms_map.cend(); s++) {
			if (*((*f).first) == *((*s).first)) {
				BOOLEAN found_value = false;
				for (STRING_LIST* expected_clause : (*f).second) {
					for (STRING_LIST* clause : (*s).second) {
						if (*clause == *expected_clause) {
							found_value = true;
						}
					}

					REQUIRE(found_value);
				}


				found_key = true;
				break;
			}
		}

		REQUIRE(found_key);
	}
}

TEST_CASE("RemoveIrrelevant | Perform AND operation between 2 data sets") {
	STRINGLIST_SET values = STRINGLIST_SET();
	STRING_LIST v = STRING_LIST();
	v.push_back("1");
	values.insert(new STRING_LIST(v));

	v.clear();
	v.push_back("2");
	values.insert(new STRING_LIST(v));
	
	v.clear();
	v.push_back("3");
	values.insert(new STRING_LIST(v));

	v.clear();
	v.push_back("4");
	values.insert(new STRING_LIST(v));

	v.clear();
	v.push_back("5");
	values.insert(new STRING_LIST(v));

	STRINGLIST_SET to_retain = STRINGLIST_SET();
	v.clear();
	v.push_back("3");
	to_retain.insert(new STRING_LIST(v));

	v.clear();
	v.push_back("1");
	to_retain.insert(new STRING_LIST(v));

	STRINGLIST_SET expected_result = STRINGLIST_SET();
	v.clear();
	v.push_back("1");
	expected_result.insert(new STRING_LIST(v));

	v.clear();
	v.push_back("3");
	expected_result.insert(new STRING_LIST(v));

	evaluator.RemoveIrrelevant(&values, to_retain, 0);
	for (auto f = expected_result.cbegin(); f != expected_result.cend(); f++) {
		BOOLEAN found = false;
		for (auto s = values.cbegin(); s != values.cend(); s++) {
			if (*(*f) == *(*s)) {
				found = true;
				break;
			}
		}

		REQUIRE(found);
	}
}

TEST_CASE("GetCartesianProduct | Perform cartesian product on data sets in order") {
	STRING_LIST output_list = { "a1", "pr1", "cl1", "s1" , "v1" };
	STRINGLIST_STRINGLISTSET_MAP results_map = STRINGLIST_STRINGLISTSET_MAP();

	STRING_LIST k1 = { "a1", "v1" };
	STRINGLIST_SET v1 = STRINGLIST_SET();
	STRING_LIST values = STRING_LIST();

	values.push_back("1"); values.push_back("var1");
	v1.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("2"); values.push_back("var2");
	v1.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("3"); values.push_back("var3");
	v1.insert(new STRING_LIST(values));

	STRING_LIST k2 = { "cl1" };
	STRINGLIST_SET v2 = STRINGLIST_SET();
	values.clear();
	values.push_back("4"); 
	v2.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("5");
	v2.insert(new STRING_LIST(values));

	STRING_LIST k3 = { "pr1", "s1" };
	STRINGLIST_SET v3 = STRINGLIST_SET();
	values.clear();
	values.push_back("6"); values.push_back("6");
	v3.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("7"); values.push_back("7");
	v3.insert(new STRING_LIST(values));

	results_map.insert({ new STRING_LIST(k1), v1 });
	results_map.insert({ new STRING_LIST(k2), v2 });
	results_map.insert({ new STRING_LIST(k3), v3 });

	STRINGLIST_SET expected_result = STRINGLIST_SET();
	values.clear();
	values.push_back("1"); values.push_back("6"); values.push_back("4"); values.push_back("6"); values.push_back("var1");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("1"); values.push_back("6"); values.push_back("5"); values.push_back("6"); values.push_back("var1");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("1"); values.push_back("7"); values.push_back("4"); values.push_back("7"); values.push_back("var1");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("1"); values.push_back("7"); values.push_back("5"); values.push_back("7"); values.push_back("var1");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("2"); values.push_back("6"); values.push_back("4"); values.push_back("6"); values.push_back("var2");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("2"); values.push_back("6"); values.push_back("5"); values.push_back("6"); values.push_back("var2");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("2"); values.push_back("7"); values.push_back("4"); values.push_back("7"); values.push_back("var2");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("3"); values.push_back("7"); values.push_back("5"); values.push_back("7"); values.push_back("var3");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("3"); values.push_back("6"); values.push_back("4"); values.push_back("6"); values.push_back("var3");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("3"); values.push_back("6"); values.push_back("5"); values.push_back("6"); values.push_back("var3");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("3"); values.push_back("7"); values.push_back("4"); values.push_back("7"); values.push_back("var3");
	expected_result.insert(new STRING_LIST(values));
	values.clear();
	values.push_back("3"); values.push_back("7"); values.push_back("5"); values.push_back("7"); values.push_back("var3");
	expected_result.insert(new STRING_LIST(values));

	STRINGLIST_SET result = evaluator.GetCartesianProduct(results_map, output_list);
	for (auto f = expected_result.cbegin(); f != expected_result.cend(); f++) {
		BOOLEAN found = false;
		for (auto s = result.cbegin(); s != result.cend(); s++) {
			if (*(*f) == *(*s)) {
				found = true;
				break;
			}
		}

		REQUIRE(found);
	}
}
<<<<<<< HEAD

TEST_CASE("no clauses || return stmts") {
	QueryInfo queryInfo;
	QueryResult result;
	unordered_set<string> expectedResult = { "1", "2", "3" };

	unordered_map<string, string> varMap;
	varMap.insert({ "read", TYPE_STMT_READ });
	varMap.insert({ "p", TYPE_STMT_PRINT });
	varMap.insert({ "while", TYPE_STMT_WHILE });
	varMap.insert({ "ifs", TYPE_STMT_IF });
	varMap.insert({ "c", TYPE_STMT_CALL });
	varMap.insert({ "a", TYPE_STMT_ASSIGN });
	varMap.insert({ "s", TYPE_STMT });
	queryInfo.SetVarMap(varMap);

	unordered_map<string, vector<vector<string>>> relRefMap = {};
	queryInfo.SetRelRefMap(relRefMap);

	queryInfo.SetOutputVar({ "read" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);

	queryInfo.SetOutputVar({ "p" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);

	queryInfo.SetOutputVar({ "while" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);

	queryInfo.SetOutputVar({ "ifs" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);

	queryInfo.SetOutputVar({ "c" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);

	queryInfo.SetOutputVar({ "a" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);

	queryInfo.SetOutputVar({ "s" });
	result = evaluator.Evaluate(queryInfo);
	REQUIRE(result.GetResult() == expectedResult);
}
*/
