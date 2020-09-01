#include "TNode.h"

TNode::TNode(void) {

}

TNode::TNode(NodeType _type, Name _name, Value _value, Operator _op, Index _index) 
	: type(_type), value(_value), name(_name), op(_op), index(_index)
{
	children = new list<TNode*>;
};

bool TNode::AddChild(TNode* child)
{
	if (child == NULL) {
		return false;
	}
	(*children).push_back(child);
	return true;
};

bool TNode::SetParent(TNode* newParent)
{
	if (newParent == NULL) {
		return false;
	}
	parent = newParent;
	return true;
};

bool TNode::SetName(string newName)
{
	if (type != varName && type != procName) {
		return false;
	}
	name = newName;
	return true;
};

bool TNode::SetValue(double newValue)
{
	if (type != constValue) {
		return false;
	}
	value = newValue;
	return true;
};

bool TNode::SetOp(Operator newOp)
{
	if (type != relExpr && type != condExpr && type != expr && type != term) {
		return false;
	}
	op = newOp;
	return true;
};