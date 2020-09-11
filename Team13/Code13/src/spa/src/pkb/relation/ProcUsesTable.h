#ifndef PROCUSESTABLE_H
#define PROCUSESTABLE_H

#include <unordered_map>
#include <CustomTypes.h>

class ProcUsesTable {
private:
    std::unordered_map<PROC_NAME*, VAR_NAME_SET*> data;
public:
    ProcUsesTable() {
        data = std::unordered_map<PROC_NAME*, VAR_NAME_SET*>();
    }
    bool addProcUses(PROC_NAME* p, VAR_NAME* v);
    bool IsProcUses(PROC_NAME* p, VAR_NAME* v);
    VAR_NAME_SET* GetProcUses(PROC_NAME* p);

};
#endif // PROCUSESTABLE_H