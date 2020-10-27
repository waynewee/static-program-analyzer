#include "GUINode.h"

GUINode::GUINode(TNode* node, TNode* parent, int x, int y) {
    node_ = node;
    parent_ = parent;
    x_ = x;
    y_ = y;
}

GUINode::GUINode(int stmt_idx,int x, int y) {
    stmt_idx_ = stmt_idx;
    x_ = x;
    y_ = y;
}

