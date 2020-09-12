#include "pql\PQLEvaluator.h"
#include "catch.hpp"
#include <iostream>
#include "pkb/PKB.h"

PQLEvaluator evaluator;

TEST_CASE("no clauses || return variables") {	
	unordered_map<string, string> varMap;
	varMap.insert({ "v", TYPE_VAR });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "v" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "var1", "var2" };

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "proc1", "proc2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return const || without API call (not done)") {
	unordered_map<string, string> varMap;
	varMap.insert({ "c", TYPE_CONST });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "c" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = 
		{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return assign stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "a", TYPE_STMT_ASSIGN });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "a" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return if stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "ifs", TYPE_STMT_IF });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "ifs" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "3", "4" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return while stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "while", TYPE_STMT_WHILE });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "while" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "5", "6" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return read stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "read", TYPE_STMT_READ });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "read" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "7", "8" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return print stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "p", TYPE_STMT_PRINT });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "p" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "9", "10" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("no clauses || return call stmt") {
	unordered_map<string, string> varMap;
	varMap.insert({ "c", TYPE_STMT_CALL });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "c" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "11", "12" };

	REQUIRE(result.getResult() == expectedResult);
}


TEST_CASE("Checking if pointer works correctly") {
	// create set
	unordered_set<string*> set = unordered_set<string*>();
	string k1 = "abc";
	set.insert(&k1);
	unordered_set<string*>* key = &set;

	REQUIRE(key->size() == 1);
}

TEST_CASE("Checking if PKB works correctly") {
	PKB pkb;
	unordered_set<string*>* key = pkb.GetAllVariables();
	REQUIRE(key->size() == 2);
}


