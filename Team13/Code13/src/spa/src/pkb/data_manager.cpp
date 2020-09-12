//
// Created by Xu Lin on 12/9/20.
//

#include "data_manager.h"
bool DataManager::AddStatement(STATEMENT_TYPE t, STMT_IDX s) {
    return stmt_table->Add(t, s);
}
STMT_IDX_SET *DataManager::GetAllStatements(STATEMENT_TYPE t) {
    return stmt_table->GetAll(t);
}
STMT_IDX_SET *DataManager::GetAllStatements() {
    return stmt_table->GetAll();
}
bool DataManager::AddVariable(VAR_NAME *v) {
    return var_table->Add(v);
}
VAR_NAME_SET *DataManager::GetAllVariables() {
    return var_table->GetAll();
}
bool DataManager::AddProcedure(PROC_NAME *p) {
    return proc_table->Add(p);
}
PROC_NAME_SET *DataManager::GetAllProcedures() {
    return proc_table->GetAll();
}
