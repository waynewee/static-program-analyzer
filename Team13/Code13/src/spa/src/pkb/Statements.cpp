//
// Created by Xu Lin on 8/9/20.
//
#include <iterator>
#include "Statements.h"

using namespace std;

bool Statements::addStatement(STATEMENT_TYPE t, STMT_IDX s) {
    if (data[t]->insert(&s).second || data[ALL]->insert(&s).second) {
        return true;
    }
    return false;
}

STMT_IDX_SET* Statements::GetAllStatements(STATEMENT_TYPE t) {
    switch (t) {
        case assignStatement:
            return data[ASSIGN];
        case whileStatement:
            return data[WHILE];
        case readStatement:
            return data[READ];
        case printStatement:
            return data[PRINT];
        case ifStatement:
            return data[IF];
        case callStatement:
            return data[CALL];
    }
    return nullptr;
}

STMT_IDX_SET* Statements::GetAllStatements() {
    return data[ALL];
}
