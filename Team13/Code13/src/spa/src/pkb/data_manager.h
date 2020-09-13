#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <CustomTypes.h>
#include "data_tables.h"

class DataManager {
private:
    ConstTable *const_table_;
    StatementTable *stmt_table_;
    VariableTable *var_table_;
    ProcedureTable *proc_table_;

public:
    DataManager() {
        const_table_ = new ConstTable();
        stmt_table_ = new StatementTable();
        var_table_ = new VariableTable();
        proc_table_ = new ProcedureTable();
    }

    bool AddStatement(STATEMENT_TYPE t, STMT_IDX s);
    STMT_IDX_SET* GetAllStatements(STATEMENT_TYPE t);
    STMT_IDX_SET* GetAllStatements();

    bool AddVariable(VAR_NAME *v);
    VAR_NAME_SET* GetAllVariables();

    bool AddProcedure(PROC_NAME *p);
    PROC_NAME_SET* GetAllProcedures();

    bool AddConstant(CONST_VALUE c);
    CONST_VALUE_SET* GetAllConstants();
};

#endif // DATA_MANAGER_H