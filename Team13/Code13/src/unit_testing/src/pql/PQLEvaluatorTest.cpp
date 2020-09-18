#include "pql/PQLEvaluator.h"
#include "catch.hpp"
#include <iostream>
#include "pkb/PKB.h"
#include "pql/PQLDriver.h"

PQLEvaluator evaluator;
PKB pkb = PKB();
/*
TEST_CASE("lazy testing") {
	PQLDriver pql = PQLDriver();

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetDataManager()->AddStatement(ifStatement, 3);
	pkb.GetDataManager()->AddStatement(whileStatement, 4);
	string* proc = new string("proc");
	pkb.GetDataManager()->AddProcedure(proc);
	string* var = new string("var");
	pkb.GetDataManager()->AddVariable(var);
	pkb.GetRelationManager()->AddFollows(1, 2);
	pkb.GetRelationManager()->AddFollowsStar(1, 2);
	pkb.GetRelationManager()->AddFollows(2, 3);
	pkb.GetRelationManager()->AddFollowsStar(2, 3);

	pkb.GetRelationManager()->AddStmtUses(1, var);
	pkb.GetRelationManager()->AddStmtModifies(1, var);
	pkb.GetRelationManager()->AddProcModifies(proc, var);
	pkb.GetRelationManager()->AddProcUses(proc, var);

	pql.query("stmt s1, s2; Select s1 such that Follows(s1, s2)");
	pql.query("stmt s1, s2; Select s1 such that Follows*(s1, s2)");
	pql.query("stmt s1, s2; Select s1 such that Parent(s1, s2)");
	pql.query("stmt s1, s2; Select s1 such that Parent*(s1, s2)");
	pql.query("stmt s1; variable v1; Select s1 such that Uses(s1, v1)");
	pql.query("proc p1; variable v1; Select s1 such that Uses(p1, v1)");
	pql.query("stmt s1; variable v1; Select s1 such that Modifies(s1, v1)");
	pql.query("proc p1; variable v1; Select s1 such that Modifies(p1, v1)");
}

TEST_CASE("1 clause || Follows || user-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s1", TYPE_STMT });
	varMap.insert({ "s2", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s1", "s2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s1" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || user-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || wildcard-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "s"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || user-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || int-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"1", "s"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || wildcard-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || wildcard-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || int-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"2", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(2, 1);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || int-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"1", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetRelationManager()->AddFollows(1, 2);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2"};

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return variables") {
	unordered_map<string, string> varMap;
	varMap.insert({ "v", TYPE_VAR });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "v" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	string* v1 = new string("v1");
	string* v2 = new string("v2");
	string* v3 = new string("v3");
	pkb.GetDataManager()->AddVariable(v1);
	pkb.GetDataManager()->AddVariable(v2);
	pkb.GetDataManager()->AddVariable(v3);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "v1", "v2", "v3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return procedures") {
	unordered_map<string, string> varMap;
	varMap.insert({ "p", TYPE_PROC });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "p" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	string* p1 = new string("p1");
	string* p2 = new string("p2");
	string* p3 = new string("p3");
	pkb.GetDataManager()->AddProcedure(p1);
	pkb.GetDataManager()->AddProcedure(p2);
	pkb.GetDataManager()->AddProcedure(p3);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "p1", "p2", "p3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return const") {
	unordered_map<string, string> varMap;
	varMap.insert({ "c", TYPE_CONST });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "c" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	pkb.reset();
	pkb.GetDataManager()->AddConstant(1);
	pkb.GetDataManager()->AddConstant(2);
	pkb.GetDataManager()->AddConstant(3);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return stmts") {
	QueryInfo queryInfo;
	QueryResult result;
	unordered_set<string> expectedResult;

	pkb.reset();
	pkb.GetDataManager()->AddStatement(assignStatement, 1);
	pkb.GetDataManager()->AddStatement(assignStatement, 2);
	pkb.GetDataManager()->AddStatement(assignStatement, 3);

	pkb.GetDataManager()->AddStatement(callStatement, 4);
	pkb.GetDataManager()->AddStatement(callStatement, 5);
	pkb.GetDataManager()->AddStatement(callStatement, 6);

	pkb.GetDataManager()->AddStatement(ifStatement, 7);
	pkb.GetDataManager()->AddStatement(ifStatement, 8);
	pkb.GetDataManager()->AddStatement(ifStatement, 9);

	pkb.GetDataManager()->AddStatement(whileStatement, 10);
	pkb.GetDataManager()->AddStatement(whileStatement, 11);
	pkb.GetDataManager()->AddStatement(whileStatement, 12);

	pkb.GetDataManager()->AddStatement(printStatement, 13);
	pkb.GetDataManager()->AddStatement(printStatement, 14);
	pkb.GetDataManager()->AddStatement(printStatement, 15);

	pkb.GetDataManager()->AddStatement(readStatement, 16);
	pkb.GetDataManager()->AddStatement(readStatement, 17);
	pkb.GetDataManager()->AddStatement(readStatement, 18);

	unordered_map<string, string> varMap;
	varMap.insert({ "read", TYPE_STMT_READ });
	varMap.insert({ "p", TYPE_STMT_PRINT });
	varMap.insert({ "while", TYPE_STMT_WHILE });
	varMap.insert({ "ifs", TYPE_STMT_IF });
	varMap.insert({ "c", TYPE_STMT_CALL });
	varMap.insert({ "a", TYPE_STMT_ASSIGN });
	varMap.insert({ "s", TYPE_STMT });
	queryInfo.setVarMap(varMap);

	unordered_map<string, vector<vector<string>>> relRefMap = {};
	queryInfo.setRelRefMap(relRefMap);

	queryInfo.setOutputVar({ "read" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "16", "17", "18" };
	REQUIRE(result.getResult() == expectedResult);

	queryInfo.setOutputVar({ "p" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "13", "14", "15" };
	REQUIRE(result.getResult() == expectedResult);

	queryInfo.setOutputVar({ "while" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "10", "11", "12" };
	REQUIRE(result.getResult() == expectedResult);

	queryInfo.setOutputVar({ "ifs" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "7", "8", "9" };
	REQUIRE(result.getResult() == expectedResult);

	queryInfo.setOutputVar({ "c" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "4", "5", "6" };
	REQUIRE(result.getResult() == expectedResult);

	queryInfo.setOutputVar({ "a" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "1", "2", "3" };
	REQUIRE(result.getResult() == expectedResult);

	queryInfo.setOutputVar({ "s" });
	result = evaluator.evaluate(queryInfo);
	expectedResult = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };

	REQUIRE(result.getResult() == expectedResult);
}*/