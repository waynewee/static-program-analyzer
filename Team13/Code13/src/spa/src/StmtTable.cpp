//
// Created by Xu Lin on 1/9/20.
//

#include <StmtTable.h>

using namespace std;
TNode* STMT_TABLE::Get(STMT_IDX idx) {
    auto val = data.find(idx);
    if (!(val == data.end())) return val->second;
    return nullptr;
}

void* STMT_TABLE::Add(STMT_IDX idx, TNode* tnode) {
    data.insert({idx, tnode});
    return NULL;
}

LIST_OF_STMT_IDX STMT_TABLE::GetAllStmts() {
    LIST_OF_STMT_IDX stmts;
    for(auto & it : data) {
        stmts.push_back(it.first);
    }
    return stmts;
}