//
// Created by Xu Lin on 31/8/20.
//
#include <VAR_TABLE.h>

using namespace std;

LIST_OF_TNODE_PTR VAR_TABLE::get(VAR_NAME var_name) {
    auto val = data.find(var_name);
    if (!(val == data.end())) return val->second;
    return LIST_OF_TNODE_PTR();
}

void* VAR_TABLE::add(VAR_NAME name, TNode* tnode) {
    if (!(data.find(name) == data.end())) {
        data.insert({name, vector<TNode*>()});
    }
    LIST_OF_TNODE_PTR vec = data.find(name)->second;
    vec.push_back(tnode);
    return NULL;
}

LIST_OF_VAR_NAME VAR_TABLE::getAllVars() {
    LIST_OF_VAR_NAME vars;
    for(auto & it : data) {
        vars.push_back(it.first);
    }
    return vars;
}