//
// Created by Xu Lin on 5/10/20.
//

#include "CFG.h"
bool CFG::AddEdge(STMT_IDX s1, STMT_IDX s2) {
    auto result = AddEdgeUtil(data_, all_edges_, s1, s2);
    if (result) {
        AddKey(s1);
    }
    return result;
}
bool CFG::AddInverseEdge(STMT_IDX s1, STMT_IDX s2) {
    auto result = AddEdgeUtil(inverse_data_, s1, s2);
    if (result) {
        AddInverseKey(s1);
    }
    return result;
}
bool CFG::AddKey(STMT_IDX s) {
    return AddKeyUtil(data_keys_, s);
}
bool CFG::AddInverseKey(STMT_IDX s) {
    return AddKeyUtil(inverse_data_keys_, s);
}
STMT_IDX_SET CFG::GetAllAdjacencyListKeys() {
    return data_keys_;
}
STMT_IDX_SET CFG::GetAllInverseAdjacencyListKeys() {
    return inverse_data_keys_;
}
STMT_IDX_SET CFG::GetAllNeighbors(STMT_IDX s1) {
    return *data_.at(s1);
}
STMT_IDX_SET CFG::GetAllInverseNeighbors(STMT_IDX s1) {
    return *inverse_data_.at(s1);
}
bool CFG::HasEdge(STMT_IDX s1, STMT_IDX s2) {
    return HasEdgeUtil(data_, s1, s2);
}
bool CFG::HasInverseEdge(STMT_IDX s1, STMT_IDX s2) {
    return HasEdgeUtil(inverse_data_, s1, s2);
}
bool CFG::RemoveEdge(STMT_IDX s1, STMT_IDX s2) {
    auto iter = data_.find(s1);
    if (iter != data_.end()) {
        auto result = iter->second->erase(s2);
        if (iter->second->empty()) {
            iter = data_.erase(iter);
            return result && iter->second;
        }
        return result;
    }
    return false;
}
CFG_ADJACENCY_LIST& CFG::GetAdjacencyList() {
    return data_;
}
CFG_ADJACENCY_LIST& CFG::GetInverseAdjacencyList() {
    return inverse_data_;
}
bool CFG::AddEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_STMT_PAIR_LIST pairs, STMT_IDX s1, STMT_IDX s2) {
    auto iter = table.find(s1);
    bool result = true;
    if (iter == table.end()) {
        auto set = new STMT_IDX_SET();
        result = set->insert(s2).second && table.insert({ s1, set }).second;
    } else {
        auto set = iter->second;
        result = set->insert(s2).second;
    }
    if (result) {
        STMT_STMT_PAIR pair = {s1, s2};
        pairs.push_back(pair);
    }
    return result;
}
bool CFG::AddEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_IDX s1, STMT_IDX s2) {
    auto iter = table.find(s1);
    if (iter == table.end()) {
        auto set = new STMT_IDX_SET();
        return set->insert(s2).second && table.insert({ s1, set }).second;
    } else {
        auto set = iter->second;
        return set->insert(s2).second;
    }
}
bool CFG::HasEdgeUtil(CFG_ADJACENCY_LIST &table, STMT_IDX s1, STMT_IDX s2) {
    auto pair = table.find(s1);
    if (pair == table.end()) {
        return false;
    }
    return pair->second->find(s2) != pair->second->end();
}
bool CFG::AddKeyUtil(STMT_IDX_SET &keys, STMT_IDX s) {
    return keys.insert(s).second;
}
STMT_STMT_PAIR_LIST CFG::GetAllEdeges() {
    return all_edges_;
}

