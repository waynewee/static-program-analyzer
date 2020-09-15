//
// Created by Xu Lin on 11/9/20.
//

#include "RelationManager.h"
FOLLOWS_TABLE* RelationManager::follows_table_ = new FOLLOWS_TABLE();
FOLLOWS_STAR_TABLE* RelationManager::follows_star_table_ = new FOLLOWS_STAR_TABLE();
INVERSE_FOLLOWS_TABLE* RelationManager::inverse_follows_table_ = new INVERSE_FOLLOWS_TABLE();
INVERSE_FOLLOWS_STAR_TABLE* RelationManager::inverse_follows_star_table_ = new INVERSE_FOLLOWS_STAR_TABLE();
PARENT_TABLE* RelationManager::parent_table_ = new PARENT_TABLE();
PARENT_STAR_TABLE* RelationManager::parent_star_table_ = new PARENT_STAR_TABLE();
INVERSE_PARENT_TABLE* RelationManager::inverse_parent_table_ = new INVERSE_PARENT_TABLE();
INVERSE_PARENT_STAR_TABLE* RelationManager::inverse_parent_star_table_ = new INVERSE_PARENT_STAR_TABLE();
STMT_USES_TABLE* RelationManager::stmt_uses_table_ = new STMT_USES_TABLE();
PROC_USES_TABLE* RelationManager::proc_uses_table_ = new PROC_USES_TABLE();
INVERSE_STMT_USES_TABLE* RelationManager::inverse_stmt_uses_table_ = new INVERSE_STMT_USES_TABLE();
INVERSE_PROC_USES_TABLE* RelationManager::inverse_proc_uses_table_ = new INVERSE_PROC_USES_TABLE();
STMT_MODIFIES_TABLE* RelationManager::stmt_modifies_table_ = new STMT_MODIFIES_TABLE();
PROC_MODIFIES_TABLE* RelationManager::proc_modifies_table_ = new PROC_MODIFIES_TABLE();
INVERSE_STMT_MODIFIES_TABLE* RelationManager::inverse_stmt_modifies_table_ = new INVERSE_STMT_MODIFIES_TABLE();
INVERSE_PROC_MODIFIES_TABLE* RelationManager::inverse_proc_modifies_table_ = new INVERSE_PROC_MODIFIES_TABLE();
STMT_STMT_PAIR_SET* RelationManager::all_follows_ = new STMT_STMT_PAIR_SET();
STMT_STMT_PAIR_SET* RelationManager::all_follows_star_ = new STMT_STMT_PAIR_SET();
STMT_STMT_PAIR_SET* RelationManager::all_parent_ = new STMT_STMT_PAIR_SET();
STMT_STMT_PAIR_SET* RelationManager::all_parent_star_ = new STMT_STMT_PAIR_SET();
STMT_VAR_PAIR_SET* RelationManager::all_stmt_uses_ = new STMT_VAR_PAIR_SET();
PROC_VAR_PAIR_SET* RelationManager::all_proc_uses_ = new PROC_VAR_PAIR_SET();
STMT_VAR_PAIR_SET* RelationManager::all_stmt_modifies_ = new STMT_VAR_PAIR_SET();
PROC_VAR_PAIR_SET* RelationManager::all_proc_modifies_ = new PROC_VAR_PAIR_SET();
bool RelationManager::AddFollows(STMT_IDX s1, STMT_IDX s2) {

    return InsertStmtStmtRelation(follows_table_, s1, s2)
            && InsertStmtStmtRelation(inverse_follows_table_, s2, s1)
            && InsertStmtStmtRelation(follows_star_table_, s1, s2)
            && InsertStmtStmtRelation(inverse_follows_star_table_, s2, s1)
            && InsertStmtStmtTuple(all_follows_, s1, s2)
            && InsertStmtStmtTuple(all_follows_star_, s1, s2);
}

bool RelationManager::AddFollowsStar(STMT_IDX s1, STMT_IDX s2) {
    return InsertStmtStmtRelation(follows_star_table_, s1, s2)
            && InsertStmtStmtRelation(inverse_follows_star_table_, s2, s1)
            && InsertStmtStmtTuple(all_follows_star_, s1, s2);
}

bool RelationManager::AddParent(STMT_IDX s1, STMT_IDX s2) {
    return InsertStmtStmtRelation(parent_table_, s1, s2)
            && InsertStmtStmtRelation(inverse_parent_table_, s2, s1)
            && InsertStmtStmtRelation(parent_star_table_, s1, s2)
            && InsertStmtStmtRelation(inverse_parent_star_table_, s2, s1)
            && InsertStmtStmtTuple(all_parent_, s1, s2)
            && InsertStmtStmtTuple(all_parent_star_, s1, s2);
}
bool RelationManager::AddParentStar(STMT_IDX s1, STMT_IDX s2) {
    return InsertStmtStmtRelation(parent_star_table_, s1, s2)
            && InsertStmtStmtRelation(inverse_parent_star_table_, s2, s1)
            && InsertStmtStmtTuple(all_parent_star_, s1, s2);
}
bool RelationManager::AddStmtUses(STMT_IDX s, VAR_NAME *v) {
    return InsertStmtVarRelation(stmt_uses_table_, s, v)
            && InsertVarStmtRelation(inverse_stmt_uses_table_, v, s)
            && InsertStmtVarTuple(all_stmt_uses_, s, v);
}
bool RelationManager::AddProcUses(PROC_NAME *p, VAR_NAME *v) {
    return InsertProcVarRelation(proc_uses_table_, p, v)
            && InsertVarProcRelation(inverse_proc_uses_table_, v, p)
            && InsertProcVarTuple(all_proc_uses_, p, v);
}
bool RelationManager::AddStmtModifies(STMT_IDX s, VAR_NAME *v) {
    return InsertStmtVarRelation(stmt_modifies_table_, s, v)
            && InsertVarStmtRelation(inverse_stmt_modifies_table_, v, s)
            && InsertStmtVarTuple(all_stmt_modifies_, s, v);
}
bool RelationManager::AddProcModifies(PROC_NAME *p, VAR_NAME *v) {
    return InsertProcVarRelation(proc_modifies_table_, p, v)
            && InsertVarProcRelation(inverse_proc_modifies_table_, v, p)
            && InsertProcVarTuple(all_proc_modifies_, p, v);
}

bool RelationManager::IsFollows(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(follows_table_, s1, s2);
}
bool RelationManager::IsFollowsStar(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(follows_star_table_, s1, s2);
}
bool RelationManager::IsParent(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(parent_table_, s1, s2);
}
bool RelationManager::IsParentStar(STMT_IDX s1, STMT_IDX s2) {
    return CheckStmtStmtRelation(parent_star_table_, s1, s2);
}
bool RelationManager::IsStmtUses(STMT_IDX s, VAR_NAME *v) {
    return CheckStmtVarRelation(stmt_uses_table_, s, v);
}
bool RelationManager::IsProcUses(PROC_NAME *p, VAR_NAME *v) {
    return CheckProcVarRelation(proc_uses_table_, p, v);
}
bool RelationManager::IsStmtModifies(STMT_IDX s, VAR_NAME *v) {
    return CheckStmtVarRelation(stmt_modifies_table_, s, v);
}
bool RelationManager::IsProcModifies(PROC_NAME *p, VAR_NAME *v) {
    return CheckProcVarRelation(proc_modifies_table_, p, v);
}
STMT_IDX_SET *RelationManager::GetFollows(STMT_IDX s) {
    return GetStmtStmtRelationVal(follows_table_, s);
}
STMT_IDX_SET *RelationManager::GetInverseFollows(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_follows_table_, s);
}
STMT_IDX_SET *RelationManager::GetFollowsStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(follows_star_table_, s);
}
STMT_IDX_SET *RelationManager::GetInverseFollowsStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_follows_star_table_, s);
}
STMT_IDX_SET *RelationManager::GetParents(STMT_IDX s) {
    return GetStmtStmtRelationVal(parent_table_, s);
}
STMT_IDX_SET *RelationManager::GetInverseParents(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_parent_table_, s);
}
STMT_IDX_SET *RelationManager::GetParentStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(parent_star_table_, s);
}
STMT_IDX_SET *RelationManager::GetInverseParentStars(STMT_IDX s) {
    return GetStmtStmtRelationVal(inverse_parent_star_table_, s);
}
VAR_NAME_SET *RelationManager::GetStmtUses(STMT_IDX s) {
    return GetStmtVarRelationVal(stmt_uses_table_, s);
}
STMT_IDX_SET *RelationManager::GetInverseStmtUses(VAR_NAME *v) {
    return GetVarStmtRelationVal(inverse_stmt_uses_table_, v);
}
VAR_NAME_SET *RelationManager::GetProcUses(PROC_NAME *p) {
    return GetProcVarRelationVal(proc_uses_table_, p);
}
PROC_NAME_SET * RelationManager::GetInverseProcUses(VAR_NAME *v) {
    return GetVarProcRelationVal(inverse_proc_uses_table_, v);
}
VAR_NAME_SET *RelationManager::GetStmtModifies(STMT_IDX s) {
    return GetStmtVarRelationVal(stmt_modifies_table_, s);
}
STMT_IDX_SET *RelationManager::GetInverseStmtModifies(VAR_NAME *v) {
    return GetVarStmtRelationVal(inverse_stmt_uses_table_, v);
}
VAR_NAME_SET *RelationManager::GetProcModifies(PROC_NAME *p) {
    return GetProcVarRelationVal(proc_uses_table_, p);
}
PROC_NAME_SET * RelationManager::GetInverseProcModifies(VAR_NAME *v) {
    return GetVarProcRelationVal(inverse_proc_uses_table_, v);
}
STMT_STMT_PAIR_SET *RelationManager::GetAllFollows() {
    return all_follows_;
}
STMT_STMT_PAIR_SET *RelationManager::GetAllFollowsStars() {
    return all_follows_star_;
}
STMT_STMT_PAIR_SET *RelationManager::GetAllParents() {
    return all_parent_;
}
STMT_STMT_PAIR_SET *RelationManager::GetAllParentStars() {
    return all_parent_star_;
}
STMT_VAR_PAIR_SET *RelationManager::GetAllStmtUses() {
    return all_stmt_uses_;
}
PROC_VAR_PAIR_SET *RelationManager::GetAllProcUses() {
    return all_proc_uses_;
}
STMT_VAR_PAIR_SET *RelationManager::GetAllStmtModifies() {
    return all_stmt_modifies_;
}
PROC_VAR_PAIR_SET *RelationManager::GetAllProcModifies() {
    return all_proc_modifies_;
}

bool RelationManager::InsertStmtStmtRelation(std::unordered_map<STMT_IDX, STMT_IDX_SET*> *set, STMT_IDX s1, STMT_IDX s2) {
    auto iter = set->find(s1);
    if (iter == set->end()) {
        STMT_IDX_SET* stmt_set = new STMT_IDX_SET();
        return set->insert({s1, stmt_set}).second && stmt_set->insert(s2).second;
    } else {
        return set->at(s1)->insert(s2).second;
    }
}

bool RelationManager::InsertStmtVarRelation(STMT_VAR_RELATION_TABLE *set, STMT_IDX s, VAR_NAME *v) {
    auto iter = set->find(s);
    if (iter == set->end()) {
        VAR_NAME_SET* var_name_set = new VAR_NAME_SET();
        return set->insert({s, var_name_set}).second && var_name_set->insert(v).second;
    } else {
        return set->at(s)->insert(v).second;
    }
}

bool RelationManager::InsertProcVarRelation(PROC_VAR_RELATION_TABLE *set, PROC_NAME *p, VAR_NAME *v) {
    auto iter = set->find(p);
    if (iter == set->end()) {
        VAR_NAME_SET* var_name_set = new VAR_NAME_SET();
        return set->insert({p, var_name_set}).second && var_name_set->insert(v).second;
    } else {
        return set->at(p)->insert(v).second;
    }
}

bool RelationManager::InsertVarStmtRelation(VAR_STMT_RELATION_TABLE *set, VAR_NAME *v, STMT_IDX s) {
    auto iter = set->find(v);
    if (iter == set->end()) {
        STMT_IDX_SET* stmt_idx_set = new STMT_IDX_SET();
        return set->insert({v, stmt_idx_set}).second && stmt_idx_set->insert(s).second;
    } else {
        return set->at(v)->insert(s).second;
    }
}

bool RelationManager::InsertVarProcRelation(VAR_PROC_RELATION_TABLE *set, VAR_NAME *v, PROC_NAME *p) {
    auto iter = set->find(v);
    if (iter == set->end()) {
        PROC_NAME_SET* proc_name_set = new PROC_NAME_SET();
        return set->insert({v, proc_name_set}).second && proc_name_set->insert(p).second;
    } else {
        return set->at(v)->insert(p).second;
    }
}
bool RelationManager::CheckStmtStmtRelation(STMT_STMT_RELATION_TABLE *set, STMT_IDX s1, STMT_IDX s2) {
    auto iter = set->find(s1);
    return iter != set->end() && iter->second->find(s2) != iter->second->end();
}
bool RelationManager::CheckStmtVarRelation(STMT_VAR_RELATION_TABLE *set, STMT_IDX s, VAR_NAME *v) {
    auto iter = set->find(s);
    return iter != set->end() && iter->second->find(v) != iter->second->end();
}
bool RelationManager::CheckProcVarRelation(PROC_VAR_RELATION_TABLE *set, PROC_NAME *p, VAR_NAME *v) {
    auto iter = set->find(p);
    return iter != set->end() && iter->second->find(v) != iter->second->end();
}
bool RelationManager::CheckVarStmtRelation(VAR_STMT_RELATION_TABLE *set, VAR_NAME *v, STMT_IDX s) {
    auto iter = set->find(v);
    return iter != set->end() && iter->second->find(s) != iter->second->end();
}
bool RelationManager::CheckVarProcRelation(VAR_PROC_RELATION_TABLE *set, VAR_NAME *v, PROC_NAME *p) {
    auto iter = set->find(v);
    return iter != set->end() && iter->second->find(p) != iter->second->end();
}
STMT_IDX_SET* RelationManager::GetStmtStmtRelationVal(STMT_STMT_RELATION_TABLE *set, STMT_IDX s) {
    auto iter = set->find(s);
    if (iter != set->end())  {
        return iter->second;
    }
    return new STMT_IDX_SET();
}
VAR_NAME_SET* RelationManager::GetStmtVarRelationVal(STMT_VAR_RELATION_TABLE *set, STMT_IDX s) {
    auto iter = set->find(s);
    if (iter != set->end())  {
        return iter->second;
    }
    return new VAR_NAME_SET();
}
VAR_NAME_SET* RelationManager::GetProcVarRelationVal(PROC_VAR_RELATION_TABLE *set, PROC_NAME *p) {
    auto iter = set->find(p);
    if (iter != set->end())  {
        return iter->second;
    }
    return new VAR_NAME_SET();
}
STMT_IDX_SET* RelationManager::GetVarStmtRelationVal(VAR_STMT_RELATION_TABLE *set, VAR_NAME *v) {
    auto iter = set->find(v);
    if (iter != set->end())  {
        return iter->second;
    }
    return new STMT_IDX_SET();
}
PROC_NAME_SET* RelationManager::GetVarProcRelationVal(VAR_PROC_RELATION_TABLE *set, VAR_NAME *v) {
    auto iter = set->find(v);
    if (iter != set->end())  {
        return iter->second;
    }
    return new PROC_NAME_SET();
}
bool RelationManager::InsertStmtStmtTuple(STMT_STMT_PAIR_SET *set, STMT_IDX s1, STMT_IDX s2) {
    STMT_STMT_PAIR tuple = std::make_tuple(s1, s2);
    return set->insert(&tuple).second;
}
bool RelationManager::InsertStmtVarTuple(STMT_VAR_PAIR_SET *set, STMT_IDX s, VAR_NAME *v) {
    STMT_VAR_PAIR tuple = std::make_tuple(s, v);
    return set->insert(&tuple).second;
}
bool RelationManager::InsertProcVarTuple(PROC_VAR_PAIR_SET *set, PROC_NAME *p, VAR_NAME *v) {
    PROC_VAR_PAIR tuple = std::make_tuple(p, v);
    return set->insert(&tuple).second;
}
