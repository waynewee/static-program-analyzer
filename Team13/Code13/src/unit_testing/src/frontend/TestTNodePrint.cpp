#include "TNode.h"
#include "catch.hpp"
#include "CustomTypes.h"
#include "DataTables.h"

using namespace std;

// No comparison done. Need eyeballing.
TEST_CASE("Test AST print") {
    TNode list = TNode::TNode(TNode::stmtList);
    TNode s1 = TNode::TNode(TNode::printStmt, 2);
    TNode s2 = TNode::TNode(TNode::readStmt, 3);
    TNode s3 = TNode::TNode(TNode::assignStmt, 4);
    TNode v1 = TNode::TNode(TNode::varName, string("v"));
    TNode c1 = TNode::TNode(TNode::constValue, CONST_VALUE(2));
    list.AddChild(&s1);
    list.AddChild(&s2);
    list.AddChild(&s3);
    s3.AddChild(&v1);
    s3.AddChild(&c1);
    list.Print(&list);
    REQUIRE(1 == 1);
}