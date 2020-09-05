//
// Created by Xu Lin on 31/8/20.
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <CustomTypes.h>
#include <TNode.h>

typedef class StmtTable {
private:
    std::unordered_map<STMT_IDX, TNode*> data;
public:
    TNode* get(STMT_IDX idx);
    LIST_OF_STMT_IDX getAllStmts();
    void* add(STMT_IDX idx, TNode* tnode);
}STMT_TABLE;

