//
// Created by Xu Lin on 12/9/20.
//

#include "DataManager.h"
ConstTable* DataManager::const_table_ = new ConstTable();
StatementTable* DataManager::stmt_table_ = new StatementTable();
VariableTable* DataManager::var_table_ = new VariableTable();
ProcedureTable* DataManager::proc_table_ = new ProcedureTable();

bool DataManager::AddStatement(STATEMENT_TYPE t, STMT_IDX s) {
    return stmt_table_->Add(t, s);
}
STMT_IDX_SET *DataManager::GetAllStatements(STATEMENT_TYPE t) {
    return stmt_table_->GetAll(t);
}
STMT_IDX_SET *DataManager::GetAllStatements() {
    return stmt_table_->GetAll();
}
bool DataManager::AddVariable(VAR_NAME *v) {
    return var_table_->Add(v);
}
VAR_NAME_SET *DataManager::GetAllVariables() {
    return var_table_->GetAll();
}
bool DataManager::AddProcedure(PROC_NAME *p) {
    return proc_table_->Add(p);
}
PROC_NAME_SET *DataManager::GetAllProcedures() {
    return proc_table_->GetAll();
}
bool DataManager::AddConstant(CONST_VALUE c) {
    return const_table_->Add(c);
}
CONST_VALUE_SET *DataManager::GetAllConstants() {
    return const_table_->GetAll();
}
ConstTable *DataManager::GetConstTable() {
    return const_table_;
}
StatementTable *DataManager::GetStmtTable() {
    return stmt_table_;
}
VariableTable *DataManager::GetVarTable() {
    return var_table_;
}
ProcedureTable *DataManager::GetProcTable() {
    return proc_table_;
}
