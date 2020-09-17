
#include "TNode.h"

#include "catch.hpp"
#include "PKB.h"
#include "CustomTypes.h"
#include "DataTables.h"
#include "RelationManager.h"

using namespace std;
void require(bool b) {
    REQUIRE(b);
}

//TEST_CASE("1st Test") {
//
//    TNode T;
//
//
//
//    require(1 == 1);
//}


TEST_CASE("Checking if PKB works correctly") {
    PKB pkb;
    DataManager* dm = pkb.GetDataManager();
    VAR_NAME_SET* vars = dm->GetAllVariables();

    REQUIRE(vars->size() == 0);
}