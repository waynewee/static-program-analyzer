//
// Created by Xu Lin on 31/8/20.
#ifndef _STMT_TABLE_H_
#define _STMT_TABLE_H_

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include "TNode.h"
#include <CustomTypes.h>

typedef class StmtTable {
private:
    std::unordered_map<STMT_IDX, TNode*> data;
public:
    TNode* Get(STMT_IDX idx);
    STMT_IDX_LIST GetAllStmts();
    void* Add(STMT_IDX idx, TNode* tnode);
}STMT_TABLE;

#endif

