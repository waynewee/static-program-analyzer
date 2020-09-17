#include "TNode.h"

TNode::TNode() {

}

TNode::TNode(TNode::NODE_TYPE _type, string _name) :
	type(_type), name(_name) {
}

TNode::TNode(TNode::NODE_TYPE _type, CONST_VALUE _value) :
	type(_type), value(_value) {
}

TNode::TNode(TNode::NODE_TYPE _type, TNode::OPERATOR _op) :
	type(_type), op(_op) {
}

TNode::TNode(TNode::NODE_TYPE _type, STMT_IDX _idx) :
	type(_type), index(_idx) {
}

bool TNode::AddChild(TNode* child) {
	if (child == NULL) {
		return false;
	}
	children.push_back(child);
	return true;
};

bool TNode::SetParent(TNode* newParent) {
	if (newParent == NULL) {
		return false;
	}
	parent = newParent;
	return true;
};

bool TNode::SetName(VAR_NAME* newName) {
	if (type != varName && type != procName) {
		return false;
	}
	name = string(*newName);
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
TNode::TNode(TNode::NODE_TYPE _type) :
	type(_type) {
}

list<TNode*> TNode::GetChildrenList() {
	list<TNode*> result(children.begin(), children.end());
	return result;
}

vector<TNode*> TNode::GetChildrenVector() {
	return children;
}

string TNode::getData() {
	switch (type) {
	case TNode::program:
		return "Program";
	case TNode::procedure:
		return "Procedure";
	case TNode::stmtList:
		return "StmtList";
	case TNode::readStmt:
		return "ReadStmt#" + to_string(index);
	case TNode::printStmt:
		return "PrintStmt#" + to_string(index);
	case TNode::callStmt:
		return "CallStmt#" + to_string(index);
	case TNode::whileStmt:
		return "WhileStmt#" + to_string(index);
	case TNode::ifStmt:
		return "IfStmt#" + to_string(index);
	case TNode::assignStmt:
		return "AssignStmt#" + to_string(index);
	case TNode::condExpr:
		return "CondExpr:" + to_string(op);
	case TNode::relExpr:
		return "RelExpr:" + to_string(op);
	case TNode::relFactor:
		return "RelFactor";
	case TNode::expr:
		return "Expr:" + to_string(op);
	case TNode::term:
		return "Term:" + to_string(op);
	case TNode::factor:
		return "Factor";
	case TNode::varName:
		return "VarName:" + name;
	case TNode::procName:
		return "ProcName:" + name;
	case TNode::constValue:
		return "ConstValue:" + to_string(int(value));
	default:
		return "Unknown Node";
	}
}

void TNode::Print(TNode* root) {
	BTTree<TNode> printer(root, &TNode::GetChildrenList, &TNode::getData);
	printer.print();
}

TNode::NODE_TYPE TNode::GetNodeType() {
	return type;
}

STMT_IDX TNode::GetStmtIndex() {
	return index;
}