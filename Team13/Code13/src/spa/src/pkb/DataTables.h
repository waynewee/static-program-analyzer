#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <CustomTypes.h>

class ProcedureTable {
private:
    PROC_NAME_SET* data_;
public:
    ProcedureTable() {
        data_ = new PROC_NAME_SET();
    }
    bool Add(PROC_NAME *p);
    PROC_NAME_SET* GetAll();
};

class StatementTable {
#define _ALL_ 0
#define _ASSIGN_ 1
#define _READ_ 2
#define _PRINT_ 3
#define _CALL_ 4
#define _WHILE_ 5
#define _IF_ 6
#define _size_ 7
private:
    std::vector<STMT_IDX_SET *> data_;
public:
    StatementTable() {
        for (int i = 0; i < _size_; i++) {
            data_.push_back(new STMT_IDX_SET());
        }
    }
    bool Add(STATEMENT_TYPE t, STMT_IDX s);
    STMT_IDX_SET* GetAll(STATEMENT_TYPE t);
    STMT_IDX_SET* GetAll();
};

class VariableTable {
private:
    VAR_NAME_SET* data_;
public:
    VariableTable() {
        data_ = new VAR_NAME_SET();
    }
    bool Add(VAR_NAME *v);
    VAR_NAME_SET* GetAll();
};

class ConstTable {
private:
    CONST_VALUE_SET* data_;
public:
    ConstTable() {
        data_ = new CONST_VALUE_SET();
    }
    bool Add(CONST_VALUE cv);
    CONST_VALUE_SET* GetAll();
};

#endif // DATA_TABLES_H