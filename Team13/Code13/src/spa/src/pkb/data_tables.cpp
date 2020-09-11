//
// Created by Xu Lin on 12/9/20.
//

#include "data_tables.h"

using namespace std;

PROC_NAME_SET* ProcedureTable::GetAll() {
    return data;
}

bool StatementTable::Add(STATEMENT_TYPE t, STMT_IDX s) {
    return data[t]->insert(s).second && data[_ALL_]->insert(s).second;
}

STMT_IDX_SET* StatementTable::GetAll(STATEMENT_TYPE t) {
    switch (t) {
        case assignStatement:
            return data[_ASSIGN_];
        case whileStatement:
            return data[_WHILE_];
        case readStatement:
            return data[_READ_];
        case printStatement:
            return data[_PRINT_];
        case ifStatement:
            return data[_IF_];
        case callStatement:
            return data[_CALL_];
    }
    return nullptr;
}

STMT_IDX_SET* StatementTable::GetAll() {
    return data[_ALL_];
}

VAR_NAME_SET* VariableTable::GetAll() {
    return data;
}
bool VariableTable::Add(VAR_NAME *v) {
    return data->insert(v).second;
}
