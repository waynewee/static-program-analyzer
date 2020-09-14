#ifndef _TNODE_H_
#define _TNODE_H_

#include <string>
#include <vector>
#include <CustomTypes.h>

using namespace std;


class TNode {

public:
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
        notOp,
        andOp,
        orOp,
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

    TNode();
	TNode(NODE_TYPE _type);
	TNode(NODE_TYPE _type, string *_name); //var name or proc name node
	TNode(NODE_TYPE _type, CONST_VALUE _value); //const value node
	TNode(NODE_TYPE _type, OPERATOR _op); //operator node
	TNode(NODE_TYPE _type, STMT_IDX _idx); //statement node
	bool AddChild(TNode* child);
	bool SetParent(TNode* newParent);
	bool SetName(VAR_NAME *newName);
	bool SetValue(CONST_VALUE newValue);
	bool SetOp(OPERATOR newOp);

private:
    NODE_TYPE type;
    TNode* parent;
    vector<TNode*>* children;
    CONST_VALUE value;
    string* name;
    OPERATOR op;
    STMT_IDX index;
};


#endif