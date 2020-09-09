#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <pkb/PKB.h>
#include <pkb/TNode.h>

int PKB::SetProcToAST(TNode* r) {
	return 0;
}

TNode *PKB::GetRootAST(){
	return nullptr;
}

bool PKB::IsFollows(STMT_IDX s1, STMT_IDX s2)
{
	return false;
}

bool PKB::IsFollowsStar(STMT_IDX s1, STMT_IDX s2)
{
	return false;
}

bool PKB::IsParent(STMT_IDX s1, STMT_IDX s2)
{
	return false;
}

bool PKB::IsParentStar(STMT_IDX s1, STMT_IDX s2)
{
	return false;
}

bool PKB::IsStmtUses(STMT_IDX s, VAR_NAME *v)
{
	return false;
}

bool PKB::IsProcUses(PROC_NAME *p, VAR_NAME *v) {
    return false;
}

bool PKB::IsStmtModifies(STMT_IDX s, VAR_NAME *v) {
    return false;
}

bool PKB::IsProcModifies(PROC_NAME *p, VAR_NAME *v) {
    return false;
}

STMT_IDX_SET *PKB::GetFollows(STMT_IDX s1, STMT_IDX s2)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetFollowsStar(STMT_IDX s1, STMT_IDX s2)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetParent(STMT_IDX s1, STMT_IDX s2)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetParentStar(STMT_IDX s1, STMT_IDX s2)
{
	return nullptr;
}

VAR_NAME_SET *PKB::GetStmtUses(STMT_IDX s) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseStmtUses(VAR_NAME *v) {
    return nullptr;
}

VAR_NAME_SET *PKB::GetProcUses(PROC_NAME *p) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseProcUses(VAR_NAME *v) {
    return nullptr;
}

VAR_NAME_SET *PKB::GetStmtModifies(STMT_IDX s) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseStmtModifies(VAR_NAME *v) {
    return nullptr;
}

VAR_NAME_SET *PKB::GetProcModifies(PROC_NAME *p) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseProcModifies(VAR_NAME *v) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetPattern(STATEMENT_TYPE *t, VAR_NAME *v, EXPRESSION *e)
{
	return nullptr;
}

VAR_NAME_SET *PKB::GetAllVariables() {
    return var_table->GetAllVariables();
}

PROC_NAME_SET *PKB::GetAllProcedures() {
    return proc_table->GetAllProcedures();
}

STMT_IDX_SET *PKB::GetAllStatements() {
    return stmt_table->GetAllStatements();
}

STMT_IDX_SET *PKB::GetAllStatements(STATEMENT_TYPE t) {
    return stmt_table->GetAllStatements(t);
}

