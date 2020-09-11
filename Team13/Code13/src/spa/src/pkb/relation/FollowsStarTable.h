//
// Created by Xu Lin on 8/9/20.
//

#ifndef _FOLLOWSSTARTABLE_H_
#define _FOLLOWSSTARTABLE_H_

#include <unordered_map>
#include <CustomTypes.h>

class FollowsStarTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    FollowsStarTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addFollowsStar(STMT_IDX s1, STMT_IDX s2);
    bool IsFollowsStar(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetFollowsStar(STMT_IDX s);
};

#endif //_FOLLOWSSTARTABLE_H_
