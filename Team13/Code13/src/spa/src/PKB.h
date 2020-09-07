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
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer
class StmtTable;

class PKB {
private:
	static VarTable* varTable;
	static StmtTable* stmtTable;
	TNode* root;

public:
	static int setProcToAST(TNode* r);
	static TNode* getRootAST();

	bool Follows(STATEMENT* s1, STATEMENT* s2);
	bool FollowsStar(STATEMENT* s1, STATEMENT* s2);
	bool Parent(STATEMENT* s1, STATEMENT* s2);
	bool ParentStar(STATEMENT* s1, STATEMENT* s2);
	bool Uses(ENTITY* e, VARIABLE* v);
	bool Modifies(ENTITY* e, VARIABLE* v);

	STATEMENT_LIST getFollows(STATEMENT* s1, STATEMENT* s2);
	STATEMENT_LIST getFollowsStar(STATEMENT* s1, STATEMENT* s2);
	STATEMENT_LIST getParent(STATEMENT* s1, STATEMENT* s2);
	STATEMENT_LIST getParentStar(STATEMENT* s1, STATEMENT* s2);
	ENTITY_LIST getUses(ENTITY* e, VARIABLE* v);
	ENTITY_LIST getModifies(ENTITY* e, VARIABLE* v);

	STATEMENT_LIST getPattern(STATEMENT_TYPE t, VARIABLE* v, EXPRESSION e);

	VARIABLE_LIST allVariables();
	PROCEDURE_LIST allProcs();
};

