#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef CUSTOM_TYPE_HEADER
#define CUSTOM_TYPE_HEADER

#include <CustomTypes.h>

#endif 

using namespace std;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer
class StmtTable;

class PKB {
private:
	static VarTable* var_table;
	static StmtTable* stmt_table;
	TNode* root;

public:
	static int SetProcToAST(TNode* r);
	static TNode* GetRootAST();

	bool IsFollows(STATEMENT* s1, STATEMENT* s2);
	bool IsFollowsStar(STATEMENT* s1, STATEMENT* s2);
	bool IsParent(STATEMENT* s1, STATEMENT* s2);
	bool IsParentStar(STATEMENT* s1, STATEMENT* s2);
	bool IsUses(ENTITY* e, VARIABLE* v);
	bool IsModifies(ENTITY* e, VARIABLE* v);

	STATEMENT_LIST GetFollows(STATEMENT* s1, STATEMENT* s2);
	STATEMENT_LIST GetFollowsStar(STATEMENT* s1, STATEMENT* s2);
	STATEMENT_LIST GetParent(STATEMENT* s1, STATEMENT* s2);
	STATEMENT_LIST GetParentStar(STATEMENT* s1, STATEMENT* s2);
	ENTITY_LIST GetUses(ENTITY* e, VARIABLE* v);
	ENTITY_LIST GetModifies(ENTITY* e, VARIABLE* v);

	STATEMENT_LIST GetPattern(STATEMENT_TYPE t, VARIABLE* v, EXPRESSION e);

	VARIABLE_LIST GetAllVariables();
	PROCEDURE_LIST GetAllProcs();
};

