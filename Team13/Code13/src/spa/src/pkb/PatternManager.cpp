//
// Created by Xu Lin on 20/9/20.
//

#include "PatternManager.h"


bool PatternManager::AddAssignFullPattern(EXPRESSION *e, STMT_IDX s) {
    bool result = assign_full_pattern_table_->Add(e, s)
        && assign_sub_pattern_table_->Add(e, s);
    return result;
}

bool PatternManager::AddAssignSubpattern(EXPRESSION *e, STMT_IDX s) {
    return assign_sub_pattern_table_->Add(e, s);
}

STMT_IDX_SET *PatternManager::GetAssignWithFullPattern(VAR_NAME *v, EXPRESSION *e) {
    return GetAssignWithPattern(v, e, assign_full_pattern_table_);
}
STMT_IDX_SET *PatternManager::GetAssignWithSubpattern(VAR_NAME *v, EXPRESSION *e) {
    return GetAssignWithPattern(v, e, assign_sub_pattern_table_);
}
STMT_IDX_SET *PatternManager::GetAssignWithPattern(VAR_NAME *v, EXPRESSION *e, AssignPatternTable *table) {
    if (!v && !e) {
        return statement_table_->GetAll(assignStatement);
    }
    if (!v && e) {
        return table->Get(e);
    }
    if (v && !e) {
        STMT_IDX_LIST* output_list = GetAllAssignModifies(v);
        return new STMT_IDX_SET(output_list->begin(), output_list->end());
    }
    STMT_IDX_LIST* assign_modifies = GetAllAssignModifies(v);
    STMT_IDX_SET* assign_with_pattern = table->Get(e);
    STMT_IDX_LIST* output_list = GetStmtIdxSetIntersection(assign_modifies, assign_with_pattern);
    return new STMT_IDX_SET(output_list->begin(), output_list->end());
}

bool AssignPatternTable::Add(EXPRESSION* e, STMT_IDX s) {
    auto iter = data_->find(*e);
    if (iter != data_->end()) {
        STMT_IDX_SET* set = new STMT_IDX_SET();
        return data_->insert({*e, set}).second && set->insert(s).second;
    }
    return iter->second->insert(s).second;
}

STMT_IDX_SET* AssignPatternTable::Get(EXPRESSION *e) {
    auto iter = data_->find(*e);
    if (iter != data_->end())  {
        return iter->second;
    }
    return new STMT_IDX_SET();
}

STMT_IDX_LIST* PatternManager::GetAllAssignModifies(VAR_NAME *v) {
    STMT_IDX_LIST* output_list;
    STMT_IDX_SET* all_assigns_set = statement_table_->GetAll(assignStatement);
    STMT_IDX_SET* all_modifies_v_set = relation_manager_->GetInverseStmtModifies(v);
    return GetStmtIdxSetIntersection(all_assigns_set, all_modifies_v_set);
}

STMT_IDX_LIST* PatternManager::GetStmtIdxSetIntersection(STMT_IDX_SET* s1, STMT_IDX_SET* s2) {
    auto output = new STMT_IDX_LIST();
    auto l1 = new STMT_IDX_LIST(s1->begin(), s1->end());
    auto l2 = new STMT_IDX_LIST(s2->begin(), s2->end());
    std::sort(l1->begin(), l1->end());
    std::sort(l2->begin(), l2->end());
    std::set_intersection(l1->begin(), l1->end(), l2->begin(), l2->end(), back_inserter(*output));
    return output;
}

STMT_IDX_LIST* PatternManager::GetStmtIdxSetIntersection(STMT_IDX_LIST* l1, STMT_IDX_SET* s2) {
    auto output = new STMT_IDX_LIST();
    auto l2 = new STMT_IDX_LIST(s2->begin(), s2->end());
    std::sort(l1->begin(), l1->end());
    std::sort(l2->begin(), l2->end());
    std::set_intersection(l1->begin(), l1->end(), l2->begin(), l2->end(), back_inserter(*output));
    return output;
}