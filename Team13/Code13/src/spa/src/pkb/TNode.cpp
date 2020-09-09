#include "TNode.h"

TNode::TNode(void) {

}

TNode::TNode(NODE_TYPE _type, VAR_NAME *_name, CONST_VALUE _value, OPERATOR _op, STMT_IDX _index)
	: type(_type), value(_value), name(_name), op(_op), index(_index) {
	children = new vector<TNode*>;
};

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
};