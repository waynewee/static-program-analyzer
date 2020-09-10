#include "TNode.h"

TNode::TNode(TNode::NODE_TYPE _type, string *_name):
    type(_type), name(_name) {
    children = new vector<TNode*>;
}
TNode::TNode(TNode::NODE_TYPE _type, CONST_VALUE _value):
    type(_type), value(_value){
    children = new vector<TNode*>;

}
TNode::TNode(TNode::NODE_TYPE _type, TNode::OPERATOR _op):
    type(_type), op(_op) {
    children = new vector<TNode*>;
}
TNode::TNode(TNode::NODE_TYPE _type, STMT_IDX _idx):
    type(_type), index(_idx) {
    children = new vector<TNode*>;
}

bool TNode::AddChild(TNode* child) {
	if (child == NULL) {
		return false;
	}
	(*children).push_back(child);
	return true;
};

bool TNode::SetParent(TNode* newParent) {
	if (newParent == NULL) {
		return false;
	}
	parent = newParent;
	return true;
};

bool TNode::SetName(VAR_NAME *newName) {
	if (type != varName && type != procName) {
		return false;
	}
	name = newName;
	return true;
};

bool TNode::SetValue(CONST_VALUE newValue) {
	if (type != constValue) {
		return false;
	}
	value = newValue;
	return true;
};

bool TNode::SetOp(OPERATOR newOp) {
	if (type != relExpr && type != condExpr && type != expr && type != term) {
		return false;
	}
	op = newOp;
	return true;
}
TNode::TNode(TNode::NODE_TYPE _type):
    type(_type) {
    children = new vector<TNode*>;

}