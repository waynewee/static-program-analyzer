#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "pkb/TNode.h"

int PKB::SetProcToAST(TNode* r) {
	return 0;
}

TNode* PKB::GetRootAST(){
	return nullptr;
}

bool PKB::IsFollows(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::IsFollowsStar(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::IsParent(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::IsParentStar(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::IsUses(ENTITY* e, VARIABLE* v)
{
	return false;
}

bool PKB::IsModifies(ENTITY* e, VARIABLE* v)
{
	return false;
}

STATEMENT_LIST PKB::GetFollows(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

STATEMENT_LIST PKB::GetFollowsStar(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

STATEMENT_LIST PKB::GetParent(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

STATEMENT_LIST PKB::GetParentStar(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

ENTITY_LIST PKB::GetUses(ENTITY* e, VARIABLE* v)
{
	return ENTITY_LIST();
}

ENTITY_LIST PKB::GetModifies(ENTITY* e, VARIABLE* v)
{
	return ENTITY_LIST();
}

STATEMENT_LIST PKB::GetPattern(STATEMENT_TYPE t, VARIABLE* v, EXPRESSION e)
{
	return STATEMENT_LIST();
}

VARIABLE_LIST PKB::GetAllVariables()
{
	return VARIABLE_LIST();
}

PROCEDURE_LIST PKB::GetAllProcs()
{
	return PROCEDURE_LIST();
}