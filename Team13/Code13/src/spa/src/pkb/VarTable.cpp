//
// Created by Xu Lin on 31/8/20.
//
//#include <pkb/storage/VarTable.h>
//
//using namespace std;
//
//LIST_OF_TNODE_PTR VAR_TABLE::Get(VAR_NAME var_name) {
//    auto val = data.find(var_name);
//    if (!(val == data.end())) return val->second;
//    return LIST_OF_TNODE_PTR();
//}
//
//void* VAR_TABLE::Add(VAR_NAME name, TNode* tnode) {
//    if (!(data.find(name) == data.end())) {
//        data.insert({name, vector<TNode*>()});
//    }
//    LIST_OF_TNODE_PTR vec = data.find(name)->second;
//    vec.push_back(tnode);
//    return NULL;
//}
//
//VAR_NAME_LIST VAR_TABLE::GetAllVars() {
//    VAR_NAME_LIST vars;
//    for(auto & it : data) {
//        vars.push_back(it.first);
//    }
//    return vars;
//}