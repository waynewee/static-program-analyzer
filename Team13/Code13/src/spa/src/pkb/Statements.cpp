//
// Created by Xu Lin on 8/9/20.
//
#include <iterator>
#include "Statements.h"

using namespace std;

bool Statements::addStatement(STATEMENT_TYPE t, STMT_IDX s) {
    if (data[t]->insert(s).second || data[_ALL_]->insert(s).second) {
        return true;
    }
    return false;
}

STMT_IDX_SET* Statements::GetAllStatements(STATEMENT_TYPE t) {
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

STMT_IDX_SET* Statements::GetAllStatements() {
    return data[_ALL_];
}
