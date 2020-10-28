#include <string>

using namespace std;

#include <PKB.h>
#include <TNode.h>

DataManager PKB::data_manager_;
RelationManager PKB::relation_manager_;
PatternManager PKB::pattern_manager_;
CFGManager PKB::cfg_manager_;
TNode PKB::ast_;
AFFECTS_TABLE PKB::affects_table_;
INVERSE_AFFECTS_TABLE PKB::inverse_affects_table_;
DataManager PKB::GetDataManager() {
    return data_manager_;
}
RelationManager PKB::GetRelationManager() {
    return relation_manager_;
}
PatternManager PKB::GetPatternManager() {
    return pattern_manager_;
}
void PKB::SetASTRoot(TNode* root) {
    if (root->GetNodeType() != TNode::NODE_TYPE::program) {
        throw ASTRootTypeUnmatchException();
    }
    ast_ = *root;
}

TNode PKB::GetASTRoot() {
    return ast_;
}
CFG& PKB::GetCFG() {
    return cfg_manager_.GetCFG();
}
void PKB::SetCFG(CFG& cfg) {
    cfg_manager_.SetCFG(cfg);
}
CFGManager PKB::GetCFGManager() {
    return cfg_manager_;
}
STMT_IDX_SET PKB::GetAffects(STMT_IDX a) {
    //debug
    std::cout << "PKB: GetAffects(" << a << ")" << std::endl;
    if (affects_table_.find(a) != affects_table_.end()) {
        return *affects_table_.at(a);
    }
    auto lhs_var = *(relation_manager_.GetStmtModifies(a).begin());
    auto visited = new STMT_IDX_SET();
    auto result = new STMT_IDX_SET();
    visited->insert(a);
    auto neighbors = cfg_manager_.GetNext(a);
    for (auto neighbor: neighbors) {
        RecursiveGetAffects(neighbor, lhs_var, *visited, *result);
    }
    if (result->size() > 0) {
        affects_table_.insert({a, result});
    }
    //debug
    std::cout << "PKB: printing result from GetAffects(" << a << std::endl;
    for (auto e : *result) {
        std::cout << "" << e << std::endl;
    }
    return *result;
}

STMT_IDX_SET PKB::GetInverseAffects(STMT_IDX a) {
    if (inverse_affects_table_.find(a) != inverse_affects_table_.end()) {
        return *inverse_affects_table_.at(a);
    }
    auto rhs_vars = relation_manager_.GetStmtUses(a);
    auto visited = new INVERSE_AFFECTS_VISITED_SET();
    auto result = new STMT_IDX_SET();
    auto neighbors = cfg_manager_.GetInverseNext(a);
    for (auto rhs_var: rhs_vars) {
        if (visited->find(rhs_var) == visited->end()) {
            visited->at(rhs_var) = new STMT_IDX_SET();
        }
        visited->at(rhs_var)->insert(a);
        for (auto neighbor: neighbors) {
            RecursiveGetInverseAffects(neighbor, rhs_var, *visited, *result);
        }
    }
    if (result->size() > 0) {
        inverse_affects_table_.insert({a, result});
    }
    //debug
    std::cout << "PKB: printing result from GetInverseAffects(" << a << std::endl;
    for (auto e : *result) {
        std::cout << "" << e << std::endl;
    }
    return *result;

}
void PKB::RecursiveGetAffects(STMT_IDX node, VAR_NAME lhs_var, STMT_IDX_SET& visited, STMT_IDX_SET& result) {
    if (visited.find(node) != visited.end()) {
        return;
    }
    //mark as visited
    visited.insert(node);
    //If the node is an assign statement and it uses lhs_var, the statement is affected
    if (data_manager_.IsAssignStmt(node) && relation_manager_.IsStmtUses(node, lhs_var)) {
        result.insert(node);
    }
    if (data_manager_.IsAssignStmt(node) && relation_manager_.IsStmtModifies(node, lhs_var)) {
        return;
    }
    //If this is an assign statement and it modifies the variable, terminate dfs on path
    if (data_manager_.IsCallStmt(node)) {
        auto procName = data_manager_.GetCalledByStmt(node);
        auto assignStmts = data_manager_.GetAllAssignStatements(procName);
        for (auto stmt: assignStmts) {
            if (relation_manager_.IsStmtModifies(stmt, lhs_var)) {
                return;
            }
        }
    }
    auto neighbors = cfg_manager_.GetNext(node);
    for (auto neighbor: neighbors) {
        RecursiveGetAffects(neighbor, lhs_var, visited, result);
    }
}
void PKB::RecursiveGetInverseAffects(STMT_IDX node, VAR_NAME rhs_var, INVERSE_AFFECTS_VISITED_SET& visited, STMT_IDX_SET& result) {
    if (visited.at(rhs_var)->find(node) != visited.at(rhs_var)->end()) {
        return;
    }
    //mark as visited
    visited.at(rhs_var)->insert(node);
    //If the node is an assign statement and it modifies the rhs_var, then node is affected by this statement.
    if (data_manager_.IsAssignStmt(node) && relation_manager_.IsStmtModifies(node, rhs_var)) {
        result.insert(node);
        return;
    }
    //If this is an assign statement and it modifies the variable, terminate dfs on path
    if (data_manager_.IsCallStmt(node)) {
        auto procName = data_manager_.GetCalledByStmt(node);
        auto assignStmts = data_manager_.GetAllAssignStatements(procName);
        for (auto stmt: assignStmts) {
            if (relation_manager_.IsStmtModifies(stmt, rhs_var)) {
                return;
            }
        }
    }
    auto neighbors = cfg_manager_.GetInverseNext(node);
    for (auto neighbor: neighbors) {
        RecursiveGetInverseAffects(neighbor, rhs_var, visited, result);
    }
}