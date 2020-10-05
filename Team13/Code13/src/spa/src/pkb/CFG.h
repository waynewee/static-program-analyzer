#ifndef CFG_H
#define CFG_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <CustomTypes.h>

class CFG {
private:
    std::unordered_map<STMT_IDX, STMT_IDX_SET*> data_;
public:
    bool AddEdge(STMT_IDX s1, STMT_IDX s2);
    STMT_IDX_SET GetAllEdges(STMT_IDX s1);
    bool HasEdge(STMT_IDX s1, STMT_IDX s2);
};


#endif // CFG_H