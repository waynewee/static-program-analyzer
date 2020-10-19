/*
#include "pql/PQLEvaluator.h"
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

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || wildcard-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || wildcard-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || int-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"2", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || int-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"1", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "s" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}


TEST_CASE("no clauses || return variables") {
	unordered_map<string, string> varMap;
	varMap.insert({ "v", TYPE_VAR });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "v" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "v1", "v2", "v3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("no clauses || return procedures") {
	unordered_map<string, string> varMap;
	varMap.insert({ "p", TYPE_PROC });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "p" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "p1", "p2", "p3" };

	REQUIRE(result.GetResult() == expectedResult);
}

TEST_CASE("no clauses || return const") {
	unordered_map<string, string> varMap;
	varMap.insert({ "c", TYPE_CONST });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.SetOutputVar({ "c" });
	queryInfo.SetRelRefMap(relRefMap);
	queryInfo.SetVarMap(varMap);

	QueryResult result = evaluator.Evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.GetResult() == expectedResult);
}

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