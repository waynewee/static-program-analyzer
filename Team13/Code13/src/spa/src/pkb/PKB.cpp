#include <string>

using namespace std;

#include <PKB.h>
#include <TNode.h>

DataManager* PKB::data_manager_ = new DataManager();
RelationManager* PKB::relation_manager_ = new RelationManager();
TNode* PKB::ast_;
DataManager *PKB::GetDataManager() {
    return data_manager_;
}
RelationManager* PKB::GetRelationManager() {
    return relation_manager_;
}
void PKB::SetASTRoot(TNode *root) {
    if (root->GetNodeType() != TNode::NODE_TYPE::program) {
        throw ASTRootTypeUnmatchException();
    }
    ast_ = root;
}

TNode* PKB::GetASTRoot() {
    return ast_;
}
