#ifndef INVERSEPROCMODIFIESTABLE_H
#define INVERSEPROCMODIFIESTABLE_H

#include <unordered_map>
#include <CustomTypes.h>

class InverseProcModifiesTable {
private:
    std::unordered_map<VAR_NAME*, PROC_NAME_SET*> data;
public:
    InverseProcModifiesTable() {
        data = std::unordered_map<VAR_NAME*, PROC_NAME_SET*>();
    }
    bool addInverseProcModifies(VAR_NAME* v, PROC_NAME* p);
    bool isInverseProcModifies(VAR_NAME* v, PROC_NAME* p);
    VAR_NAME_SET* GetInverseProcModifies(VAR_NAME* v);
};

#endif // INVERSEPROCMODIFIESTABLE_H