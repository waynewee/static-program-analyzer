//
// Created by Xu Lin on 8/9/20.
//

#ifndef INVERSEFOLLOWSSTARTABLE_H
#define INVERSEFOLLOWSSTARTABLE_H

#include <unordered_map>
#include <CustomTypes.h>

class InverseFollowsStarTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    InverseFollowsStarTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addInverseFollowsStar(STMT_IDX s1, STMT_IDX s2);
    bool IsInverseFollowsStar(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetInverseFollowsStar(STMT_IDX s);
};

#endif //INVERSEFOLLOWSSTARTABLE_H
