#include "PKB.h"
#include "TNode.h"
#include "catch.hpp"
#include "CustomTypes.h"
#include "DesignExtractor.h"

using namespace std;

TEST_CASE("Test Add Follows") {
    TNode list = TNode::TNode(TNode::stmtList);
    TNode s1 = TNode::TNode(TNode::printStmt, 1);
    TNode s2 = TNode::TNode(TNode::readStmt, 2);
    TNode s3 = TNode::TNode(TNode::assignStmt, 3);
    TNode v1 = TNode::TNode(TNode::varName, string("v"));
    TNode c1 = TNode::TNode(TNode::constValue, CONST_VALUE(2));
    list.AddChild(&s1);
    list.AddChild(&s2);
    list.AddChild(&s3);
    s3.AddChild(&v1);
    s3.AddChild(&c1);
    list.Print(&list);
    PKB pkb;
    RelationManager* manager = pkb.GetRelationManager();
    ExtractFollows(manager, &list);
    bool trueFollow = manager->IsFollows(1, 2);
    bool falseFollow = manager->IsFollows(1, 3);
    REQUIRE(trueFollow == true);
    REQUIRE(falseFollow == false);
}

TEST_CASE("Test Add Parent Star") {
    TNode* while_ = new TNode(TNode::NODE_TYPE::whileStmt, 1);
    TNode* nested_while_ = new TNode(TNode::NODE_TYPE::whileStmt, 4);

    // while children
    TNode* whilecond = new TNode(TNode::NODE_TYPE::condExpr);
    TNode* whilelst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* s1 = new TNode(TNode::NODE_TYPE::printStmt, 2);
    TNode* s2 = new TNode(TNode::NODE_TYPE::readStmt, 3);
    whilelst->AddChild(s1);
    whilelst->AddChild(s2);
    whilelst->AddChild(nested_while_);
    while_->AddChild(whilecond);
    while_->AddChild(whilelst);

    //nested while
    TNode* nestedcond = new TNode(TNode::NODE_TYPE::condExpr);
    TNode* nestedlst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* s3 = new TNode(TNode::NODE_TYPE::printStmt, 5);
    TNode* s4 = new TNode(TNode::NODE_TYPE::readStmt, 6);
    nestedlst->AddChild(s3);
    nestedlst->AddChild(s4);
    nested_while_->AddChild(nestedcond);
    nested_while_->AddChild(nestedlst);
    
    PKB pkb;
    RelationManager* manager = pkb.GetRelationManager();
    ExtractParent(manager, while_);
    bool trueParent = manager->IsParentStar(1, 4);
    bool falseParent = manager->IsParentStar(4, 1);
    bool trueParentStar = manager->IsParentStar(1, 5);
    bool falseParentStar = manager->IsParentStar(2, 5);
    REQUIRE(trueParent);
    REQUIRE(!falseParent);
    REQUIRE(trueParentStar);
    REQUIRE(!falseParentStar);
}

// FIXME: Uncatched vector out of bound exception when if has no else
TEST_CASE("Test Add Parent") {
    TNode* while_ = new TNode(TNode::NODE_TYPE::whileStmt, 1);
    TNode* if_ = new TNode(TNode::NODE_TYPE::ifStmt, 5);

    // while children
    TNode* whilecond = new TNode(TNode::NODE_TYPE::condExpr);
    TNode* whilelst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* s1 = new TNode(TNode::NODE_TYPE::printStmt, 2);
    TNode* s2 = new TNode(TNode::NODE_TYPE::readStmt, 3);
    TNode* s3 = new TNode(TNode::NODE_TYPE::assignStmt, 4);
    whilelst->AddChild(s1);
    whilelst->AddChild(s2);
    whilelst->AddChild(s3);
    while_->AddChild(whilecond);
    while_->AddChild(whilelst);

    TNode* ifcond = new TNode(TNode::NODE_TYPE::condExpr);
    TNode* iflst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* elselst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* s4 = new TNode(TNode::NODE_TYPE::printStmt, 6);
    TNode* s5 = new TNode(TNode::NODE_TYPE::readStmt, 7);
    TNode* s6 = new TNode(TNode::NODE_TYPE::readStmt, 8);
    iflst->AddChild(s4);
    iflst->AddChild(s5);
    elselst->AddChild(s6);
    if_->AddChild(ifcond);
    if_->AddChild(iflst);
    if_->AddChild(elselst);

    TNode* list = new TNode(TNode::NODE_TYPE::stmtList);
    list->AddChild(while_);
    list->AddChild(if_);
    // list.Print(&list);

    PKB pkb;
    RelationManager* manager = pkb.GetRelationManager();
    ExtractParent(manager, list);
    bool trueParentWhile = manager->IsParent(1, 2);
    bool falseParentWhile = manager->IsParent(1, 7);
    bool trueParentIf = manager->IsParent(5, 6);
    bool falseParentIf = manager->IsParent(5, 1);
    REQUIRE(trueParentWhile);
    REQUIRE(!falseParentWhile);
    REQUIRE(trueParentIf);
    REQUIRE(!falseParentIf);
}


// Failing because Relation Manager compares varName pointers in stead of value
TEST_CASE("Test Add Uses") {
    VAR_NAME* v1 = new string("v1");
    VAR_NAME* v2 = new string("v2");
    PROC_NAME* procn = new string("pp");
    
    TNode* program = new TNode(TNode::NODE_TYPE::program);

    TNode* proc = new TNode(TNode::NODE_TYPE::procedure);
    TNode* procname = new TNode(TNode::NODE_TYPE::procName, *procn);
    TNode* proclst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* procassign = new TNode(TNode::NODE_TYPE::assignStmt, 1);
    TNode* var = new TNode(TNode::NODE_TYPE::varName, *v1);
    TNode* expr = new TNode(TNode::NODE_TYPE::varName, *v2);

    procassign->AddChild(var);
    procassign->AddChild(expr);
    proclst->AddChild(procassign);
    proc->AddChild(procname);
    proc->AddChild(proclst);
    program->AddChild(proc);

    PKB pkb;
    RelationManager* manager = pkb.GetRelationManager();
    ExtractUses(manager, program);

    /*
    VAR_NAME* v1 = new string("v1");
    VAR_NAME* v2 = new string("v2");
    PROC_NAME* procn = new string("pp");
    manager->AddStmtUses(1, v1);
    manager->AddProcUses(procn, v);
    */

    bool trueStmtUses = manager->IsStmtUses(1, v1);
    bool falseStmtUses = manager->IsStmtUses(1, v2);
    bool trueProcUses = manager->IsProcUses(procn, v1);
    bool falseProcUses = manager->IsProcUses(procn, v2);

    REQUIRE(trueStmtUses);
    REQUIRE(!falseStmtUses);
    REQUIRE(trueProcUses);
    REQUIRE(!falseProcUses);
}