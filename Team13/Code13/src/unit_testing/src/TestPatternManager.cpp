//
// Created by Xu Lin on 22/9/20.
//


#include <PKB.h>
#include "catch.hpp"
#include "CustomTypes.h"

TEST_CASE("Test AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root)") {
    PKB* pkb = new PKB();
    PatternManager manager = pkb->GetPatternManager();
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

    STMT_IDX_SET set =  manager.GetAssignWithFullPattern("var1", "5");
    REQUIRE(!set.empty());
    REQUIRE(set.size() == 1);
    REQUIRE(set.find(1) != set.end());
}