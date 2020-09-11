//
// Created by Xu Lin on 31/8/20.
//

#ifndef _VARTABLE_H_
#define _VARTABLE_H_

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <ListOfTNodePointers.h>
#include <CustomTypes.h>

typedef class VarTable {
private:
    std::unordered_map<VAR_NAME, LIST_OF_TNODE_PTR> data;
public:
    LIST_OF_TNODE_PTR Get(VAR_NAME var_name);
    VAR_NAME_LIST GetAllVars();
    void* Add(VAR_NAME name, TNode* tnode);
}VAR_TABLE;
#endif