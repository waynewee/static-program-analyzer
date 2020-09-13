#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <CustomTypes.h>
#include "data_tables.h"

class DataManager {
private:
    StatementTable *stmt_table;
    VariableTable *var_table;
    ProcedureTable *proc_table;

public:
    bool AddStatement(STATEMENT_TYPE t, STMT_IDX s);
    STMT_IDX_SET* GetAllStatements(STATEMENT_TYPE t);
    STMT_IDX_SET* GetAllStatements();

    bool AddVariable(VAR_NAME *v);
    VAR_NAME_SET* GetAllVariables();

    bool AddProcedure(PROC_NAME *p);
    PROC_NAME_SET* GetAllProcedures();
};

#endif // DATA_MANAGER_H