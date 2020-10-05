//
// Created by Xu Lin on 5/10/20.
//

#include "CFG.h"
bool CFG::AddEdge(STMT_IDX s1, STMT_IDX s2) {
    auto pair = data_.find(s1);
    if (pair == data_.end()) {
        auto set = new STMT_IDX_SET();
        return set->insert(s2).second && data_.insert({ s1, set }).second;
    }
    else {
        auto set = pair->second;
        return set->insert(s2).second;
    }
}
STMT_IDX_SET CFG::GetAllEdges(STMT_IDX s1) {
    return *data_.at(s1);
}
bool CFG::HasEdge(STMT_IDX s1, STMT_IDX s2) {
    auto pair = data_.find(s1);
    if (pair == data_.end()) {
        return false;
    }
    return pair->second->find(s2) != pair->second->end();
}