//
// Created by Xu Lin on 8/9/20.
//

#ifndef _FOLLOWSTABLE_H_
#define _FOLLOWSTABLE_H_

#include <unordered_map>
#include <CustomTypes.h>

class FollowsTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    FollowsTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addFollows(STMT_IDX s1, STMT_IDX s2);
    bool IsFollows(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetFollows(STMT_IDX s);
};

#endif //_FOLLOWSTABLE_H_
