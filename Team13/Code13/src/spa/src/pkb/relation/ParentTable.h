//
// Created by Xu Lin on 8/9/20.
//

#ifndef _PARENTTABLE_H_
#define _PARENTTABLE_H_

#include <unordered_map>
#include <CustomTypes.h>

class ParentTable {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data;
public:
    ParentTable() {
        data = std::unordered_map<STMT_IDX, STMT_IDX_SET*>();
    }
    bool addParent(STMT_IDX s1, STMT_IDX s2);
    bool IsParent(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET* GetParents(STMT_IDX s);
};

#endif //PARENTTABLE_H
