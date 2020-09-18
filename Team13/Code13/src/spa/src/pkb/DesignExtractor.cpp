#include "DesignExtractor.h"

bool ExtractFollows(RelationManager* manager, TNode* root) {
    vector<TNode*> children = root->GetChildrenVector();

    if (root->GetNodeType() == TNode::stmtList) {
        int size = children.size();
        for (int i = 0; i < size - 1; i++) {
            TNode* child = children.at(i);
            TNode* next = children.at(i + 1);
            manager->AddFollows(child->GetStmtIndex(), next->GetStmtIndex());
            for (int j = i + 1; j < size; j++) {
                next = children.at(j);
                manager->AddFollowsStar(child->GetStmtIndex(), next->GetStmtIndex());
            }
        }
    }
    else if (children.empty()) {
        return true;
    }
    else {
        for (TNode* child : children) {
            ExtractFollows(manager, child);
        }
    }
    return true;
}