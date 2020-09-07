#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"

int PKB::setProcToAST(TNode* r) {
	return 0;
}

TNode* PKB::getRootAST(){
	return nullptr;
}

bool PKB::Follows(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::FollowsStar(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::Parent(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::ParentStar(STATEMENT* s1, STATEMENT* s2)
{
	return false;
}

bool PKB::Uses(ENTITY* e, VARIABLE* v)
{
	return false;
}

bool PKB::Modifies(ENTITY* e, VARIABLE* v)
{
	return false;
}

STATEMENT_LIST PKB::getFollows(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

STATEMENT_LIST PKB::getFollowsStar(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

STATEMENT_LIST PKB::getParent(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

STATEMENT_LIST PKB::getParentStar(STATEMENT* s1, STATEMENT* s2)
{
	return STATEMENT_LIST();
}

ENTITY_LIST PKB::getUses(ENTITY* e, VARIABLE* v)
{
	return ENTITY_LIST();
}

ENTITY_LIST PKB::getModifies(ENTITY* e, VARIABLE* v)
{
	return ENTITY_LIST();
}

STATEMENT_LIST PKB::getPattern(STATEMENT_TYPE t, VARIABLE* v, EXPRESSION e)
{
	return STATEMENT_LIST();
}

VARIABLE_LIST PKB::allVariables()
{
	return VARIABLE_LIST();
}

PROCEDURE_LIST PKB::allProcs()
{
	return PROCEDURE_LIST();
}