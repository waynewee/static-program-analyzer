#include "pql\PQLEvaluator.h"
#include "catch.hpp"
#include <iostream>
#include "pkb/PKB.h"

PQLEvaluator evaluator;

TEST_CASE("PQL checking if PKB works correctly || initialize the PKB") {
	PKB pkb = PKB();
	pkb.reset();

	pkb.GetDataManager()->AddConstant(1);
	pkb.GetDataManager()->AddConstant(2);
	pkb.GetDataManager()->AddConstant(3);

	string* v1 = new string("v1");
	string* v2 = new string("v2");
	string* v3 = new string("v3");
	pkb.GetDataManager()->AddVariable(v1);
	pkb.GetDataManager()->AddVariable(v2);
	pkb.GetDataManager()->AddVariable(v3);

	string* p1 = new string("p1");
	string* p2 = new string("p2");
	string* p3 = new string("p3");
	pkb.GetDataManager()->AddProcedure(p1);
	pkb.GetDataManager()->AddProcedure(p2);
	pkb.GetDataManager()->AddProcedure(p3);

	//pkb.GetDataManager()->AddStatement(assignStatement, 1);
	//pkb.GetDataManager()->AddStatement(assignStatement, 1);
	//pkb.GetDataManager()->AddStatement(assignStatement, 1);

	//pkb.GetDataManager()->AddStatement(callStatement, 1);
	//pkb.GetDataManager()->AddStatement(callStatement, 2);
	//pkb.GetDataManager()->AddStatement(callStatement, 3);

	//pkb.GetDataManager()->AddStatement(ifStatement, 4);
	//pkb.GetDataManager()->AddStatement(ifStatement, 5);
	//pkb.GetDataManager()->AddStatement(ifStatement, 6);

	//pkb.GetDataManager()->AddStatement(whileStatement, 7);
	//pkb.GetDataManager()->AddStatement(whileStatement, 8);
	//pkb.GetDataManager()->AddStatement(whileStatement, 9);

	//pkb.GetDataManager()->AddStatement(printStatement, 10);
	//pkb.GetDataManager()->AddStatement(printStatement, 11);
	//pkb.GetDataManager()->AddStatement(printStatement, 12);

	//pkb.GetDataManager()->AddStatement(readStatement, 13);
	//pkb.GetDataManager()->AddStatement(readStatement, 14);
	//pkb.GetDataManager()->AddStatement(readStatement, 15);

	REQUIRE((pkb.GetDataManager()->GetAllConstants())->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllVariables())->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllProcedures())->size() == 3);
	/*REQUIRE((pkb.GetDataManager()->GetAllStatements())->size() == 15);

	REQUIRE((pkb.GetDataManager()->GetAllStatements(assignStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(callStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(ifStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(whileStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(printStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(readStatement))->size() == 3);*/



}
/*
TEST_CASE("1 UsesS clause || params: INT & IDENT || return variables") {
	unordered_map<string, string> varMap;
	varMap.insert({ "v", TYPE_VAR });

	unordered_map<string, vector<vector<string>>> relRefMap;

	vector<vector<string>> values = { { "1", "usedString" } };
	relRefMap.insert({ "UseS", values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "v" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "v1", "v2", "v3" };

	REQUIRE(result.getResult() == expectedResult);
}
*/
TEST_CASE("no clauses || return variables") {	
	unordered_map<string, string> varMap;
	varMap.insert({ "v", TYPE_VAR });

	unordered_map<string, vector<vector<string>>> relRefMap = {};

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "v" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3"};

	REQUIRE(result.getResult() == expectedResult);
}
/*
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
*/

TEST_CASE("Checking if pointer works correctly") {
	// create set
	unordered_set<string*> set = unordered_set<string*>();
	string k1 = "abc";
	set.insert(&k1);
	unordered_set<string*>* key = &set;

	REQUIRE(key->size() == 1);
}



