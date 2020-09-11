//
// Created by Xu Lin on 8/9/20.
//

#ifndef _INVERSEPARENTTABLE_H_
#define _INVERSEPARENTTABLE_H_

#include <unordered_map>
#include <CustomTypes.h>

class InverseParentTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    InverseParentTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addInverseParent(STMT_IDX s1, STMT_IDX s2);
    bool IsInverseParent(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetInverseParents(STMT_IDX s);
};

#endif //_INVERSEPARENTTABLE_H_
