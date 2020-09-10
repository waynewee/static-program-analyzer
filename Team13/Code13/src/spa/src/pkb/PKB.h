#pragma once

#ifndef _PKB_H_
#define _PKB_H_

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <CustomTypes.h>
#include "Statements.h"
#include "Variables.h"
#include "Procedures.h"

using namespace std;

class TNode;

class PKB {
private:
	static Variables* var_table;
	static Statements* stmt_table;
	static Procedures* proc_table;
	TNode* root;

public:
	static int SetProcToAST(TNode* r);
	static TNode* GetRootAST();

	bool IsFollows(STMT_IDX s1, STMT_IDX s2);
	bool IsFollowsStar(STMT_IDX s1, STMT_IDX s2);
	bool IsParent(STMT_IDX s1, STMT_IDX s2);
	bool IsParentStar(STMT_IDX s1, STMT_IDX s2);
	bool IsStmtUses(STMT_IDX s, VAR_NAME *v);
	bool IsProcUses(PROC_NAME *p, VAR_NAME *v);
	bool IsStmtModifies(STMT_IDX s, VAR_NAME *v);
	bool IsProcModifies(PROC_NAME *p, VAR_NAME *v);

	STMT_IDX_SET* GetFollows(STMT_IDX s1, STMT_IDX s2);
	STMT_IDX_SET* GetFollowsStar(STMT_IDX s1, STMT_IDX s2);
	STMT_IDX_SET* GetParent(STMT_IDX s1, STMT_IDX s2);
	STMT_IDX_SET* GetParentStar(STMT_IDX s1, STMT_IDX s2);
	VAR_NAME_SET* GetStmtUses(STMT_IDX s);
    VAR_NAME_SET* GetProcUses(PROC_NAME *p);
    STMT_IDX_SET* GetInverseStmtUses(VAR_NAME *v);
    STMT_IDX_SET* GetInverseProcUses(VAR_NAME *v);
    VAR_NAME_SET* GetStmtModifies(STMT_IDX s);
	VAR_NAME_SET* GetProcModifies(PROC_NAME *p);
	STMT_IDX_SET* GetInverseStmtModifies(VAR_NAME *v);
	STMT_IDX_SET* GetInverseProcModifies(VAR_NAME *v);

	STMT_IDX_SET* GetPattern(STATEMENT_TYPE *t, VAR_NAME *v, EXPRESSION *e);

	VAR_NAME_SET* GetAllVariables();
	PROC_NAME_SET* GetAllProcedures();
    STMT_IDX_SET* GetAllStatements(STATEMENT_TYPE t);
    STMT_IDX_SET* GetAllStatements();
};

#endif