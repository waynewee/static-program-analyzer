//
// Created by Xu Lin on 8/9/20.
//

#ifndef STMTUSESTABLE
#define STMTUSESTABLE

#include <unordered_map>
#include <CustomTypes.h>

class StmtUsesTable {
private:
    std::unordered_map<STMT_IDX, VAR_NAME_SET*> data;
public:
    StmtUsesTable() {
        data = std::unordered_map<STMT_IDX, VAR_NAME_SET*>();
    }
    bool addStmtUses(STMT_IDX s, VAR_NAME* v);
    bool IsStmtUses(STMT_IDX s, VAR_NAME* v);
    VAR_NAME_SET* GetStmtUses(STMT_IDX s);

};

#endif //STMT_USES_TABLE
