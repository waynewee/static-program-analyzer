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

	REQUIRE((pkb.GetDataManager()->GetAllConstants())->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllVariables())->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllProcedures())->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements())->size() == 18);

	REQUIRE((pkb.GetDataManager()->GetAllStatements(assignStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(callStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(ifStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(whileStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(printStatement))->size() == 3);
	REQUIRE((pkb.GetDataManager()->GetAllStatements(readStatement))->size() == 3);

	pkb.GetRelationManager()->AddFollows(1, 2);
	pkb.GetRelationManager()->AddFollows(2, 3);
	pkb.GetRelationManager()->AddFollows(3, 4);

	/*unordered_set<tuple<int, int>*>* result = (pkb.GetRelationManager()->GetAllFollows());
	for (tuple<int, int> const* t : *result) {
		int firstParam = get<0>(*t);
		int secondParam = get<1>(*t);
		cout << firstParam << ", " << secondParam << endl;
	}*/

	//REQUIRE((pkb.GetRelationManager()->GetAllFollows())->size() == 3);
}

TEST_CASE("TTESTTING") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s1", TYPE_STMT });
	varMap.insert({ "s2", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s1", "_"}, {"s2", "_"}, {"1", "2"}, {"_", "1"} };
	vector<vector<string>> values1 = { {"s2", "_"}, {"s1", "asdasd"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });
	relRefMap.insert({ TYPE_COND_PARENT, values });
	relRefMap.insert({ TYPE_COND_PARENT_T, values });
	relRefMap.insert({ TYPE_COND_USES_S, values1 });
	relRefMap.insert({ TYPE_COND_MODIFIES_S, values1 });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s1" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}
/*
TEST_CASE(">1 clauses || Follows-Follows* || user-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "_"}};
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows* || wildcard-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "s"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows* || user-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "3"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows* || int-user") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"1", "s"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows* || wildcard-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows*  || wildcard-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"_", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows* || int-wildcard") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"2", "_"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE(">1 clauses || Follows-Follows* || int-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"1", "2"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });
	relRefMap.insert({ TYPE_COND_FOLLOWS_T, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

	REQUIRE(result.getResult() == expectedResult);
}

TEST_CASE("1 clause || Follows || user-int") {
	unordered_map<string, string> varMap;
	varMap.insert({ "s", TYPE_STMT });

	unordered_map<string, vector<vector<string>>> relRefMap;
	vector<vector<string>> values = { {"s", "3"} };
	relRefMap.insert({ TYPE_COND_FOLLOWS, values });

	QueryInfo queryInfo;
	queryInfo.setOutputVar({ "s" });
	queryInfo.setRelRefMap(relRefMap);
	queryInfo.setVarMap(varMap);

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult = { "1", "2", "3" };

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

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

	QueryResult result = evaluator.evaluate(queryInfo);

	unordered_set<string> expectedResult =
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };;

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

	unordered_set<string> expectedResult = { "c1", "c2", "c3" };

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
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18" };

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

	unordered_set<string> expectedResult = { "1", "2", "3" };

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

	unordered_set<string> expectedResult = { "4", "5", "6" };

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

	unordered_set<string> expectedResult = { "7", "8", "9" };

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

	unordered_set<string> expectedResult = { "10", "11", "12" };

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

	unordered_set<string> expectedResult = { "13", "14", "15" };

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

	unordered_set<string> expectedResult = { "16", "17", "18" };

	REQUIRE(result.getResult() == expectedResult);
}
*/