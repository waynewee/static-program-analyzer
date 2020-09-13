//
// Created by Xu Lin on 12/9/20.
//

#include "data_tables.h"

using namespace std;

PROC_NAME_SET* ProcedureTable::GetAll() {
    return data_;
}
bool ProcedureTable::Add(PROC_NAME *p) {
    return data_->insert(p).second;
}

bool StatementTable::Add(STATEMENT_TYPE t, STMT_IDX s) {
    return data_[t]->insert(s).second && data_[_ALL_]->insert(s).second;
}

STMT_IDX_SET* StatementTable::GetAll(STATEMENT_TYPE t) {
    switch (t) {
        case assignStatement:
            return data_[_ASSIGN_];
        case whileStatement:
            return data_[_WHILE_];
        case readStatement:
            return data_[_READ_];
        case printStatement:
            return data_[_PRINT_];
        case ifStatement:
            return data_[_IF_];
        case callStatement:
            return data_[_CALL_];
    }
    return nullptr;
}

STMT_IDX_SET* StatementTable::GetAll() {
    return data_[_ALL_];
}

VAR_NAME_SET* VariableTable::GetAll() {
    return data_;
}
bool VariableTable::Add(VAR_NAME *v) {
    return data_->insert(v).second;
}
bool ConstTable::Add(CONST_VALUE cv) {
    return data_->insert(cv).second;
}
CONST_VALUE_SET *ConstTable::GetAll() {
    return data_;
}
