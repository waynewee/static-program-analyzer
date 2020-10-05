//
// Created by Xu Lin on 5/10/20.
//

#include <CFG.h>
#include "catch.hpp"

TEST_CASE("Unit Test for AddEdge()") {
    CFG *cfg = new CFG();
    cfg->AddEdge(1, 2);
    REQUIRE(cfg->HasEdge(1, 2));
}