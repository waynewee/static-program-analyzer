#include "PKB.h"
#include "TNode.h"
#include "catch.hpp"
#include "CustomTypes.h"
#include "DesignExtractor.h"

using namespace std;

TEST_CASE("Test Add Follows") {
    TNode list = TNode(TNode::stmtList);
    TNode s1 = TNode(TNode::printStmt, 1);
    TNode s2 = TNode(TNode::readStmt, 2);
    TNode s3 = TNode(TNode::assignStmt, 3);
    TNode v1 = TNode(TNode::varName, string("v"));
    TNode c1 = TNode(TNode::constValue, CONST_VALUE(2));
    list.AddChild(&s1);
    list.AddChild(&s2);
    list.AddChild(&s3);
    s3.AddChild(&v1);
    s3.AddChild(&c1);
    list.Print(&list);
    PKB pkb;
    RelationManager manager = pkb.GetRelationManager();
    ExtractFollows(manager, list);
    bool trueFollow = manager.IsFollows(1, 2);
    bool falseFollow = manager.IsFollows(1, 3);
    REQUIRE(trueFollow == true);
    REQUIRE(falseFollow == false);
}

// only tested for vars and procs, TODO: consts and stmts
TEST_CASE("Test Data Manager") {
    PKB pkb;
    DataManager manager = pkb.GetDataManager();

    VAR_NAME* v1 = new string("test_var1");
    VAR_NAME* v2 = new string("test_var2");
    PROC_NAME* procn = new string("test_proc");

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

    ExtractData(manager, *program);

    // vars
    VAR_NAME_SET vars = manager.GetAllVariables();
    int varsize = vars.size();
    for (auto v : vars) {
        cout << v << "\n";
    }
    REQUIRE(varsize == 2);

    // procs
    PROC_NAME_SET procs = manager.GetAllProcedures();
    int procsize = procs.size();
    for (auto p : procs) {
        cout << p << "\n";
    }
    REQUIRE(procsize == 1);
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
    RelationManager manager = pkb.GetRelationManager();
    ExtractParent(manager, *while_);
    bool trueParent = manager.IsParentStar(1, 4);
    bool falseParent = manager.IsParentStar(4, 1);
    bool trueParentStar = manager.IsParentStar(1, 5);
    bool falseParentStar = manager.IsParentStar(2, 5);
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
    RelationManager manager = pkb.GetRelationManager();
    ExtractParent(manager, *list);
    bool trueParentWhile = manager.IsParent(1, 2);
    bool falseParentWhile = manager.IsParent(1, 7);
    bool trueParentIf = manager.IsParent(5, 6);
    bool falseParentIf = manager.IsParent(5, 1);
    REQUIRE(trueParentWhile);
    REQUIRE(!falseParentWhile);
    REQUIRE(trueParentIf);
    REQUIRE(!falseParentIf);
}


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
    TNode* procprint = new TNode(TNode::NODE_TYPE::printStmt, 2);
    TNode* printvar = new TNode(TNode::NODE_TYPE::varName, "print_vv");

    procprint->AddChild(printvar);
    procassign->AddChild(var);
    procassign->AddChild(expr);

    proclst->AddChild(procassign);
    proclst->AddChild(procprint);

    proc->AddChild(procname);
    proc->AddChild(proclst);

    program->AddChild(proc);

    program->Print(program);

    PKB pkb;
    RelationManager manager = pkb.GetRelationManager();
    ExtractUses(manager, *program);
    
    /*VAR_NAME* v1 = new string("v1");
    VAR_NAME* v2 = new string("v2");
    PROC_NAME* procn = new string("pp");
    
    manager.AddProcUses(*procn, *v1);

    auto all_uses = manager.GetAllStmtUses();
    for (auto use : all_uses) {
        cout << use.s << " " << use.v << "\n";
    }

    auto all_proc_uses = manager.GetAllProcUses();
    for (auto use : all_proc_uses) {
        cout << use.p << " " << use.v << "\n";
    }
    */

    bool trueStmtUses = manager.IsStmtUses(1, "v2");
    bool falseStmtUses = manager.IsStmtUses(1, "v1");
    bool trueProcUses = manager.IsProcUses("pp", "v2");
    bool falseProcUses = manager.IsProcUses("pp", "v1");

    REQUIRE(trueStmtUses);
    REQUIRE(!falseStmtUses);
    REQUIRE(trueProcUses);
    REQUIRE(!falseProcUses);
}

TEST_CASE("Test Add Modifies") {
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
    TNode* procprint = new TNode(TNode::NODE_TYPE::readStmt, 2);
    TNode* printvar = new TNode(TNode::NODE_TYPE::varName, "read_vv");

    TNode* proc2 = new TNode(TNode::NODE_TYPE::procedure);
    TNode* procname2 = new TNode(TNode::NODE_TYPE::procName, "proc2");
    TNode* proclst2 = new TNode(TNode::NODE_TYPE::stmtList);
    proc2->AddChild(procname2);
    proc2->AddChild(proclst2);

    procprint->AddChild(printvar);
    procassign->AddChild(var);
    procassign->AddChild(expr);

    proclst->AddChild(procassign);
    proclst->AddChild(procprint);

    proc->AddChild(procname);
    proc->AddChild(proclst);

    program->AddChild(proc);
    program->AddChild(proc2);

    program->Print(program);

    PKB pkb;
    RelationManager manager = pkb.GetRelationManager();
    ExtractModifies(manager, *program);

    /*VAR_NAME* v1 = new string("v1");
    VAR_NAME* v2 = new string("v2");
    PROC_NAME* procn = new string("pp");

    manager.AddProcUses(*procn, *v1);
    

    auto all_mods = manager.GetAllStmtModifies();
    for (auto mod : all_mods) {
        cout << mod.s << " " << mod.v << "\n";
    }

    auto all_proc_mods = manager.GetAllProcModifies();
    for (auto mod : all_proc_mods) {
        cout << mod.p << " " << mod.v << "\n";
    }
    */

    bool trueStmtModifies = manager.IsStmtModifies(1, "v1");
    bool falseStmtModifies = manager.IsStmtModifies(2, "v1");
    bool trueProcModifies = manager.IsProcModifies("pp", "v1");
    bool falseProcModifies = manager.IsProcModifies("proc2", "v1");

    REQUIRE(trueStmtModifies);
    REQUIRE(!falseStmtModifies);
    REQUIRE(trueProcModifies);
    REQUIRE(!falseProcModifies);
}

TEST_CASE("Test Modifies with call") {
    TNode* program = new TNode(TNode::NODE_TYPE::program);

    TNode* proc = new TNode(TNode::NODE_TYPE::procedure);
    TNode* procname = new TNode(TNode::NODE_TYPE::procName, "pp");
    TNode* proclst = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* procassign = new TNode(TNode::NODE_TYPE::assignStmt, 1);
    TNode* var = new TNode(TNode::NODE_TYPE::varName, "v1");
    TNode* expr = new TNode(TNode::NODE_TYPE::varName, "v2");
    TNode* procprint = new TNode(TNode::NODE_TYPE::readStmt, 2);
    TNode* printvar = new TNode(TNode::NODE_TYPE::varName, "read_vv");
    TNode* proccall = new TNode(TNode::NODE_TYPE::callStmt, 3);
    TNode* callname = new TNode(TNode::NODE_TYPE::procName, "proc3");
    procprint->AddChild(printvar);
    procassign->AddChild(var);
    procassign->AddChild(expr);
    proccall->AddChild(callname);
    proclst->AddChild(procassign);
    proclst->AddChild(procprint);
    proclst->AddChild(proccall);
    proc->AddChild(procname);
    proc->AddChild(proclst);

    TNode* proc2 = new TNode(TNode::NODE_TYPE::procedure);
    TNode* procname2 = new TNode(TNode::NODE_TYPE::procName, "proc2");
    TNode* proclst2 = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* procprint2 = new TNode(TNode::NODE_TYPE::readStmt, 4);
    TNode* printvar2 = new TNode(TNode::NODE_TYPE::varName, "read_vv2");
    procprint2->AddChild(printvar2);
    proclst2->AddChild(procprint2);
    proc2->AddChild(procname2);
    proc2->AddChild(proclst2);

    TNode* proc3 = new TNode(TNode::NODE_TYPE::procedure);
    TNode* procname3 = new TNode(TNode::NODE_TYPE::procName, "proc3");
    TNode* proclst3 = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* proccall3 = new TNode(TNode::NODE_TYPE::callStmt, 5);
    TNode* callname3 = new TNode(TNode::NODE_TYPE::procName, "pp");
    TNode* proccall33 = new TNode(TNode::NODE_TYPE::callStmt, 6);
    TNode* callname33 = new TNode(TNode::NODE_TYPE::procName, "proc2");
    proccall3->AddChild(callname3);
    proclst3->AddChild(proccall3);
    proccall33->AddChild(callname33);
    proclst3->AddChild(proccall33);
    proc3->AddChild(procname3);
    proc3->AddChild(proclst3);

    program->AddChild(proc);
    program->AddChild(proc2);
    program->AddChild(proc3);

    program->Print(program);

    PKB pkb;
    RelationManager manager = pkb.GetRelationManager();
    ExtractModifies(manager, *program);

    /*VAR_NAME* v1 = new string("v1");
    VAR_NAME* v2 = new string("v2");
    PROC_NAME* procn = new string("pp");

    manager.AddProcUses(*procn, *v1);

    */
    auto all_mods = manager.GetAllStmtModifies();
    for (auto mod : all_mods) {
        cout << mod.s << " " << mod.v << "\n";
    }

    auto all_proc_mods = manager.GetAllProcModifies();
    for (auto mod : all_proc_mods) {
        cout << mod.p << " " << mod.v << "\n";
    }
    

    bool trueStmtModifies = manager.IsStmtModifies(1, "v1");
    bool falseStmtModifies = manager.IsStmtModifies(2, "v1");
    bool trueProcModifies = manager.IsProcModifies("pp", "v1");
    bool falseProcModifies = manager.IsProcModifies("proc2", "v1");

    REQUIRE(trueStmtModifies);
    REQUIRE(!falseStmtModifies);
    REQUIRE(trueProcModifies);
    REQUIRE(!falseProcModifies);
}

TEST_CASE("Test extract Pattern") {
    TNode* prog = new TNode(TNode::NODE_TYPE::program, "SIMPLE");
    TNode* proc = new TNode(TNode::NODE_TYPE::procedure);
    TNode* proc_name = new TNode(TNode::NODE_TYPE::procName, "main");
    TNode* stmt_list = new TNode(TNode::NODE_TYPE::stmtList);
    TNode* assign1 = new TNode(TNode::NODE_TYPE::assignStmt);
    TNode* var1 = new TNode(TNode::NODE_TYPE::varName, "var1");
    TNode* expr1 = new TNode(TNode::NODE_TYPE::expr, TNode::OPERATOR::plus);
    TNode* var2 = new TNode(TNode::NODE_TYPE::varName, "var2");
    TNode* const1 = new TNode(TNode::NODE_TYPE::constValue, 5);

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

    PKB* pkb = new PKB();
    PatternManager pattern_manager = pkb->GetPatternManager();
    ExtractPattern(pattern_manager, *prog);
//    STMT_IDX_SET stmts = manager.GetAssignWithFullPattern("", "");
//    for (STMT_IDX stmt : stmts) {
//        cout << "got pattern: " << stmt << "\n";
//    }
}
