//
// Created by Xu Lin on 8/9/20.
//

#ifndef _INVERSEPARENTSTARTABLE_H_
#define _INVERSEPARENTSTARTABLE_H_

#include <unordered_map>
#include <CustomTypes.h>

class InverseParentStarTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    InverseParentStarTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addInverseParentStar(STMT_IDX s1, STMT_IDX s2);
    bool IsInverseParentStar(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetInverseParentStars(STMT_IDX s);
};

#endif //_INVERSEPARENTSTARTABLE_H_
