//
// Created by Xu Lin on 12/9/20.
//

#include "DataTables.h"

using namespace std;

PROC_NAME_SET ProcedureTable::GetAll() {
    return data_;
}
bool ProcedureTable::Add(PROC_NAME p) {
    return data_.insert(p).second;
}

bool StatementTable::Add(STATEMENT_TYPE t, STMT_IDX s) {
    bool insert_type = false;
    switch (t) {
        case assignStatement:
            insert_type = data_[_ASSIGN_].insert(s).second;
            break;
        case whileStatement:
            insert_type = data_[_WHILE_].insert(s).second;
            break;
        case readStatement:
            insert_type = data_[_READ_].insert(s).second;
            break;
        case printStatement:
            insert_type = data_[_PRINT_].insert(s).second;
            break;
        case ifStatement:
            insert_type = data_[_IF_].insert(s).second;
            break;
        case callStatement:
            insert_type = data_[_CALL_].insert(s).second;
            break;
    }
    return insert_type && data_[_ALL_].insert(s).second;
}

STMT_IDX_SET StatementTable::GetAll(STATEMENT_TYPE t) {
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
    return STMT_IDX_SET();
}

STMT_IDX_SET StatementTable::GetAll() {
    return data_[_ALL_];
}

VAR_NAME_SET VariableTable::GetAll() {
    return data_;
}
bool VariableTable::Add(VAR_NAME v) {
    return data_.insert(v).second;
}
bool ConstTable::Add(CONST_VALUE cv) {
    return data_.insert(cv).second;
}
CONST_VALUE_SET ConstTable::GetAll() {
    return data_;
}
