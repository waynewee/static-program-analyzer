//
// Created by Xu Lin on 8/9/20.
//
#ifndef _STATEMENTS_H_
#define _STATEMENTS_H_

#include <iostream>
#include <cstdlib>
#include <vector>
#include <CustomTypes.h>



class Statements {
#define _ALL_ 0
#define _ASSIGN_ 1
#define _READ_ 2
#define _PRINT_ 3
#define _CALL_ 4
#define _WHILE_ 5
#define _IF_ 6
#define _size_ 7
private:
    std::vector<STMT_IDX_SET *> data;
public:
    Statements() {
        for (int i; i < _size_; i++) {
            STMT_IDX_SET stmtSet = STMT_IDX_SET();
            data.push_back(&stmtSet);
        }
    }
    bool addStatement(STATEMENT_TYPE t, STMT_IDX s);
    STMT_IDX_SET* GetAllStatements(STATEMENT_TYPE t);
    STMT_IDX_SET* GetAllStatements();
};

#endif