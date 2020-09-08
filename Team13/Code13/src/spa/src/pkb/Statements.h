//
// Created by Xu Lin on 8/9/20.
//
#ifndef _STATEMENTS_H_
#define _STATEMENTS_H_

#include <iostream>
#include <cstdlib>
#include <vector>
#include <CustomTypes.h>

#define ALL 0
#define ASSIGN 1
#define READ 2
#define PRINT 3
#define CALL 4
#define WHILE 5
#define IF 6
#define size 7

class Statements {

private:
    std::vector<STMT_IDX_SET *> data;
public:
    Statements() {
        for (int i; i < size; i++) {
            STMT_IDX_SET stmtSet = STMT_IDX_SET();
            data.push_back(&stmtSet);
        }
    }
    bool addStatement(STATEMENT_TYPE t, STMT_IDX s);
    STMT_IDX_SET* GetAllStatements(STATEMENT_TYPE t);
    STMT_IDX_SET* GetAllStatements();
};

#endif