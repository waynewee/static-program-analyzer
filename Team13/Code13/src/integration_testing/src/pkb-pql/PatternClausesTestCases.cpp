#include "catch.hpp"

#include "pkb/PKB.h"
#include "pql/PQLEvaluator.h"


PKB pkb_p = PKB();
PQLEvaluator pql_p = PQLEvaluator();

TEST_CASE("PATTERN") {
	/*PatternManager manager = pkb_st.GetPatternManager();
	TNode* prog = new TNode(TNode::NODE_TYPE::program, "SIMPLE");
	TNode* proc = new TNode(TNode::NODE_TYPE::procedure);
	TNode* proc_name = new TNode(TNode::NODE_TYPE::procName, "main");
	TNode* stmt_list = new TNode(TNode::NODE_TYPE::stmtList);
	TNode* assign1 = new TNode(TNode::NODE_TYPE::assignStmt, 1);
	TNode* var1 = new TNode(TNode::NODE_TYPE::varName, "var1");
	TNode* expr1 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
	TNode* var2 = new TNode(TNode::NODE_TYPE::varName, "var2");
	TNode* const1 = new TNode(TNode::NODE_TYPE::constValue, 5.0);

	prog->AddChild(proc);
	proc->AddChild(proc_name);
	proc->AddChild(stmt_list);
	stmt_list->AddChild(assign1);
	assign1->AddChild(var1);
	assign1->AddChild(expr1);
	expr1->AddChild(var2);
	expr1->AddChild(const1);
	proc->SetParent(prog);
	proc_name->SetParent(proc);
	stmt_list->SetParent(proc);
	assign1->SetParent(stmt_list);
	var1->SetParent(assign1);
	expr1->SetParent(assign1);
	var2->SetParent(expr1);
	const1->SetParent(expr1);

	manager.AddAssignPattern(1, "var1", *assign1);
	manager.AddAssignPattern(2, "var1", *assign1);
	manager.AddAssignPattern(3, "var1", *assign1);

	unordered_set<string> pkb_one_result = unordered_set<string>();
	unordered_set<string> pql_one_result = unordered_set<string>();

	// ONE user-declared entities - pattern user-declared(xx, xx)
	pkb_one_result = pql.ConvertSet(pkb_st.GetPatternManager().GetAssignWithSubPattern("v1", ""));
	pql_one_result = pql.EvaluateOneSynonymSet(TYPE_COND_MODIFIES_P, "v1");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pql.ConvertSet(pkb_st.GetPatternManager().GetAssignWithFullPattern("v1", ""));
	pql_one_result = pql.EvaluateOneSynonymSet(TYPE_COND_MODIFIES_P, "v1");
	REQUIRE(pkb_one_result == pql_one_result);





	pkb_one_result = pkb_st.GetPatternManager().GetAssignStmtVarPairWithSubPattern("v2");
	pql_one_result = pql.EvaluateInverseOneSynonymSet(TYPE_COND_MODIFIES_P, "v2");
	REQUIRE(pkb_one_result == pql_one_result);

	pkb_one_result = pkb_st.GetPatternManager().GetAssignStmtVarPairWithFullPattern("");
	pql_one_result = pql.EvaluateInverseOneSynonymSet(TYPE_COND_MODIFIES_P, "_");
	REQUIRE(pkb_one_result == pql_one_result);

	// TWO user-declared entities - pattern user-declared(user-declared, xx)
	unordered_set<vector<string>*> pkb_two_result = pql.ConvertSet(pkb_st.GetRelationManager().GetAllProcModifies());
	unordered_set<vector<string>*> pql_two_result = pql.EvaluateTwoSynonymSet(TYPE_COND_MODIFIES_P);
	isSameResult(pkb_two_result, pql_two_result);


	pql.EvaluateAssignPatternCall()
	*/
}