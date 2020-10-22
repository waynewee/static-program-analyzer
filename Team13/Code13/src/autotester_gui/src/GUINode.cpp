#include "GUINode.h"

GUINode::GUINode(TNode* node, TNode* parent, int x, int y) {
    node_ = node;
    parent_ = parent;
    x_ = x;
    y_ = y;
}