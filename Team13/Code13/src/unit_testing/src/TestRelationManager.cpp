//
// Created by Xu Lin on 14/9/20.
//

#include "catch.hpp"
#include "PKB.h"
#include "CustomTypes.h"
#include "RelationManager.h"
TEST_CASE("PKB RelationManager initialisation check") {
    PKB* pkb;
    RelationManager* relation_manager = pkb->GetRelationManager();
    REQUIRE(relation_manager != nullptr);
}

TEST_CASE("RelationManager::AddFollows(STMT_IDX s1, STMT_IDX s2) unit test") {
    PKB* pkb;
    RelationManager* relation_manager = pkb->GetRelationManager();
    relation_manager->AddFollows(1, 2);
    REQUIRE(relation_manager->IsFollows(1, 2));
    REQUIRE(relation_manager->IsFollowsStar(1, 2));
}

TEST_CASE("RelationManager::GetAllFollows() unit test") {
    PKB* pkb;
    RelationManager* relation_manager = pkb->GetRelationManager();

    relation_manager->AddFollows(1, 2);
    STMT_STMT_PAIR pair = {1, 2};
    STMT_STMT_PAIR_SET *res = relation_manager->GetAllFollows();
    for (STMT_STMT_PAIR* p: *res) {
        std::cout << p->s1 << ", " << p->s2;
    }
}