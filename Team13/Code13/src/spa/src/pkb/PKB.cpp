#include <string>

using namespace std;

#include <PKB.h>
#include <TNode.h>

DataManager PKB::data_manager_ = DataManager();
RelationManager PKB::relation_manager_ = RelationManager();
PatternManager PKB::pattern_manager_ = PatternManager();
TNode PKB::ast_;
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
CFG PKB::GetCFG(CFG cfg) {
    return cfg_;
}
void PKB::SetCFG(CFG cfg) {
    cfg_ = cfg;
}
