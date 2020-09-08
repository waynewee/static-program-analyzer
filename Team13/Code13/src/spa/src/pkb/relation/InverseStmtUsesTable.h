#ifndef INVERSESTMTUSESTABLE_H
#define INVERSESTMTUSESTABLE_H

#include <unordered_map>
#include <CustomTypes.h>

class InverseStmtUsesTable {
private:
    std::unordered_map<STMT_IDX, VAR_NAME_SET*> data;
public:
    InverseStmtUsesTable() {
        data = std::unordered_map<STMT_IDX, VAR_NAME_SET*>();
    }
    bool addStmtUses(STMT_IDX s, VAR_NAME v);
    bool IsStmtUses(STMT_IDX s, VAR_NAME v);
    VAR_NAME_SET* GetStmtUses(STMT_IDX s);

};

#endif // INVERSESTMTUSESTABLE_H