//
// Created by Xu Lin on 12/9/20.
//

#include "DataTables.h"

using namespace std;

bool CallsTable::Add(PROC_NAME pname, STMT_IDX s) {
    bool result = true;
    auto found = inverse_data_.find(pname);
    if (found == inverse_data_.end()) {
        STMT_IDX_SET set = STMT_IDX_SET();
        set.insert(s);
        result = inverse_data_.insert(make_pair(pname, set)).second;
    } else {
        result = inverse_data_.at(pname).insert(s).second;
    }
    return result && data_.insert(make_pair(s, pname)).second;
}

PROC_NAME CallsTable::GetCalledBy(STMT_IDX s) {
    auto got = data_.find(s);
    if (got == data_.end()) {
        return "";
    }
    else {
        return got->second;
    }
}

STMT_IDX_SET CallsTable::GetInverseCalledBy(PROC_NAME p) {
    auto got = inverse_data_.find(p);
    if (got == inverse_data_.end()) {
        return STMT_IDX_SET();
    }
    else {
        return got->second;
    }
}

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
            insert_type = stmt_data_[_ASSIGN_].insert(s).second;
            break;
        case whileStatement:
            insert_type = stmt_data_[_WHILE_].insert(s).second;
            break;
        case readStatement:
            insert_type = stmt_data_[_READ_].insert(s).second;
            break;
        case printStatement:
            insert_type = stmt_data_[_PRINT_].insert(s).second;
            break;
        case ifStatement:
            insert_type = stmt_data_[_IF_].insert(s).second;
            break;
        case callStatement:
            insert_type = stmt_data_[_CALL_].insert(s).second;
            break;
    }
    return insert_type && stmt_data_[_ALL_].insert(s).second;
}

STMT_IDX_SET StatementTable::GetAll(STATEMENT_TYPE t) {
    switch (t) {
        case assignStatement:
            return stmt_data_[_ASSIGN_];
        case whileStatement:
            return stmt_data_[_WHILE_];
        case readStatement:
            return stmt_data_[_READ_];
        case printStatement:
            return stmt_data_[_PRINT_];
        case ifStatement:
            return stmt_data_[_IF_];
        case callStatement:
            return stmt_data_[_CALL_];
    }
    return STMT_IDX_SET();
}

STMT_IDX_SET StatementTable::GetAll() {
    return stmt_data_[_ALL_];
}
bool StatementTable::Add(PROC_NAME p, STMT_IDX s) {
    auto iter = proc_to_assign_stmt_data_.find(p);
    bool result = true;
    if (iter == proc_to_assign_stmt_data_.end()) {
        result = proc_to_assign_stmt_data_.insert({p, STMT_IDX_SET()}).second;
    }
    if (result) {
        result = proc_to_assign_stmt_data_.at(p).insert(s).second;
        return result;
    }
    return false;
}
STMT_IDX_SET StatementTable::GetAll(PROC_NAME p) {
    auto iter = proc_to_assign_stmt_data_.find(p);
    if (iter != proc_to_assign_stmt_data_.end()) {
        return iter->second;
    }
    return STMT_IDX_SET();
}
bool StatementTable::IsAssignStmt(STMT_IDX s) {
    return stmt_data_[_ASSIGN_].find(s) != stmt_data_[_ASSIGN_].end();
}
bool StatementTable::IsCallStmt(STMT_IDX s) {
    return stmt_data_[_CALL_].find(s) != stmt_data_[_CALL_].end();
}

VAR_NAME_SET VariableTable::GetAll() {
    return data_;
}
bool VariableTable::Add(VAR_NAME v) {
    return data_.insert(v).second;
}
bool ConstTable::Add(CONST_VALUE cv) {
    return data_.insert((int) cv).second;
}
INT_SET ConstTable::GetAll() {
    return data_;
}
