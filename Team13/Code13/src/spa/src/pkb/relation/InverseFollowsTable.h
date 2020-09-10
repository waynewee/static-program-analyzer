//
// Created by Xu Lin on 8/9/20.
//

#ifndef INVERSEFOLLOWSTABLE_H
#define INVERSEFOLLOWSTABLE_H

#include <unordered_map>
#include <CustomTypes.h>

class InverseFollowsTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    InverseFollowsTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addInverseFollows(STMT_IDX s1, STMT_IDX s2);
    bool IsInverseFollows(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetInverseFollows(STMT_IDX s);
};

#endif //AUTOTESTER_TEAM13_CODE13_SRC_SPA_SRC_PKB_INVERSEFOLLOWSTABLE_H_
