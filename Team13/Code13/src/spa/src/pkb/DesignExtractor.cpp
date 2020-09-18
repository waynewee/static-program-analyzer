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

bool ExtractData(DataManager* manager, TNode* root) {
    vector<TNode*> children = root->GetChildrenVector();
    switch (root->GetNodeType()) {
    case TNode::procName:
        manager->AddProcedure(root->GetName());
        break;
    case TNode::assignStmt:
        manager->AddStatement(assignStatement, root->GetStmtIndex());
        break;
    case TNode::readStmt:
        manager->AddStatement(readStatement, root->GetStmtIndex());
        break;
    case TNode::callStmt:
        manager->AddStatement(callStatement, root->GetStmtIndex());
        break;
    case TNode::ifStmt:
        manager->AddStatement(ifStatement, root->GetStmtIndex());
        break;
    case TNode::whileStmt:
        manager->AddStatement(whileStatement, root->GetStmtIndex());
        break;
    case TNode::printStmt:
        manager->AddStatement(printStatement, root->GetStmtIndex());
        break;
    case TNode::varName:
        manager->AddVariable(root->GetName());
        break;
    case TNode::constValue:
        manager->AddConstant(root->GetConstValue());
        break;
    default:
        break;
    }
    for (TNode* child : children) {
        ExtractData(manager, child);
    }
    return true;
}