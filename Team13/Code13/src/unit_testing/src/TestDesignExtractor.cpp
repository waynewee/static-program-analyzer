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