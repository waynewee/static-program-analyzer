#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <CustomTypes.h>
#include "DataTables.h"

class DataManager {
    static ConstTable *const_table_;
    static StatementTable *stmt_table_;
    static VariableTable *var_table_;
    static ProcedureTable *proc_table_;

public:
    DataManager() {
    }
    ConstTable* GetConstTable();
    StatementTable* GetStmtTable();
    VariableTable* GetVarTable();
    ProcedureTable* GetProcTable();

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