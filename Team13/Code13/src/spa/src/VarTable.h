//
// Created by Xu Lin on 31/8/20.
//
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <CustomTypes.h>
#include <ListOfTNodePointers.h>

typedef class VarTable {
private:
    std::unordered_map<VAR_NAME, LIST_OF_TNODE_PTR> data;
public:
    LIST_OF_TNODE_PTR Get(VAR_NAME var_name);
    LIST_OF_VAR_NAME GetAllVars();
    void* Add(VAR_NAME name, TNode* tnode);
}VAR_TABLE;