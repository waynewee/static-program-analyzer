//
// Created by Xu Lin on 31/8/20.
//
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <ListOfTNodePointers.h>

#ifndef CUSTOM_TYPE_HEADER
#define CUSTOM_TYPE_HEADER

#include <CustomTypes.h>

#endif 

typedef class VarTable {
private:
    std::unordered_map<VAR_NAME, LIST_OF_TNODE_PTR> data;
public:
    LIST_OF_TNODE_PTR get(VAR_NAME var_name);
    LIST_OF_VAR_NAME getAllVars();
    void* add(VAR_NAME name, TNode* tnode);
}VAR_TABLE;