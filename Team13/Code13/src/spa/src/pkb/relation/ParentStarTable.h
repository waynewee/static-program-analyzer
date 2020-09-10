//
// Created by Xu Lin on 8/9/20.
//

#ifndef _PARENTSTARTABLE_H_
#define _PARENTSTARTABLE_H_

#include <unordered_map>
#include <CustomTypes.h>

class ParentStarTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    ParentStarTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addParentStar(STMT_IDX s1, STMT_IDX s2);
    bool IsParentStar(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetParentStars(STMT_IDX s);
};

#endif //AUTOTESTER_TEAM13_CODE13_SRC_SPA_SRC_PKB_STORAGE_PARENTSTARTABLE_H_
