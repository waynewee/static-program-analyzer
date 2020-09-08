#pragma once

#include <string>
#include <vector>

#ifndef CUSTOM_TYPE_HEADER
#define CUSTOM_TYPE_HEADER

#include <CustomTypes.h>

#endif 

using namespace std;

enum NODE_TYPE {
	program,
	procedure,
	stmtList,
	readStmt,
	printStmt,
	callStmt,
	whileStmt,
	ifStmt,
	assignStmt,
	condExpr,
	relExpr,
	relFactor,
	expr,
	term,
	factor,
	varName,
	procName,
	constValue
};

enum OPERATOR {
	not,
	and,
	or ,
	greater,
	geq,
	lesser,
	leq,
	equal,
	unequal,
	plus,
	minus,
	times,
	divide,
	mod
};

class TNode {
public:
	TNode(void);
	TNode(NODE_TYPE _type, VAR_NAME _name, CONST_VALUE _value, OPERATOR _op, STMT_IDX _index);
	bool AddChild(TNode* child);
	bool SetParent(TNode* newParent);
	bool SetName(VAR_NAME newName);
	bool SetValue(CONST_VALUE newValue);
	bool SetOp(OPERATOR newOp);

private:
	NODE_TYPE type;
	TNode* parent;
	vector<TNode*>* children;
	CONST_VALUE value;
	VAR_NAME name;
	OPERATOR op;
	STMT_IDX index;
};