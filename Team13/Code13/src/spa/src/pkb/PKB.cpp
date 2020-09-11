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

STMT_IDX_SET *PKB::GetFollows(STMT_IDX s)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetInverseFollows(STMT_IDX s)
{
    return nullptr;
}

STMT_PAIR_SET *PKB::GetAllFollows()
{
    return nullptr;
}

STMT_IDX_SET *PKB::GetFollowsStars(STMT_IDX s)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetInverseFollowsStars(STMT_IDX s)
{
    return nullptr;
}

STMT_PAIR_SET *PKB::GetAllFollowsStars()
{
    return nullptr;
}

STMT_IDX_SET *PKB::GetParents(STMT_IDX s)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetInverseParents(STMT_IDX s)
{
    return nullptr;
}

STMT_PAIR_SET *PKB::GetAllParents()
{
    return nullptr;
}

STMT_IDX_SET *PKB::GetParentStars(STMT_IDX s)
{
	return nullptr;
}

STMT_IDX_SET *PKB::GetInverseParentStars(STMT_IDX s)
{
    return nullptr;
}

STMT_PAIR_SET *PKB::GetAllParentStars()
{
    return nullptr;
}

VAR_NAME_SET *PKB::GetStmtUses(STMT_IDX s) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseStmtUses(VAR_NAME *v) {
    return nullptr;
}

STMT_VAR_PAIR_SET *PKB::GetAllStmtUses() {
    return nullptr;
}

VAR_NAME_SET *PKB::GetProcUses(PROC_NAME *p) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseProcUses(VAR_NAME *v) {
    return nullptr;
}

PROC_VAR_PAIR_SET* PKB::GetAllProcUses() {
    return nullptr;
}

VAR_NAME_SET *PKB::GetStmtModifies(STMT_IDX s) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseStmtModifies(VAR_NAME *v) {
    return nullptr;
}

STMT_VAR_PAIR_SET *PKB::GetAllStmtModifies() {
    return nullptr;
}

VAR_NAME_SET *PKB::GetProcModifies(PROC_NAME *p) {
    return nullptr;
}

STMT_IDX_SET *PKB::GetInverseProcModifies(VAR_NAME *v) {
    return nullptr;
}

PROC_VAR_PAIR_SET *PKB::GetAllProcModifies() {
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

