#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <cstdlib>
#include <stack>
#include <algorithm>
#include <CustomTypes.h>
#include <DataTables.h>
#include <RelationManager.h>
#include "CustomTypes.h"
#include "PQLCustomTypes.h"
#include "TNode.h"

typedef struct {
    STMT_IDX s;
    VAR_NAME v;
    TNode eroot;
}ASSIGN_PATTERN;


typedef std::vector<ASSIGN_PATTERN> EXPRESSION_TABLE;
class AssignPatternTable {
private:
    EXPRESSION_TABLE* data_ = new EXPRESSION_TABLE();
public:
    EXPRESSION_TABLE* GetData();
    void Add(STMT_IDX s, VAR_NAME v, TNode root);
};

class PatternManager {
private:
    AssignPatternTable assign_pattern_table_;

    STMT_IDX_LIST GetStmtIdxSetIntersection(STMT_IDX_SET s1, STMT_IDX_SET s2);
    STMT_IDX_LIST GetStmtIdxSetIntersection(STMT_IDX_LIST l1, STMT_IDX_SET s2);

    STMT_IDX_SET GetAssignWithPattern(VAR_NAME v, EXPRESSION e, AssignPatternTable& table);
    STMT_IDX_SET GetStmtOfMatchingAssignments(VAR_NAME v, EXPRESSION e, EXPRESSION_TABLE* data);
    STMT_VAR_PAIR_LIST GetStmtVarPairOfMatchingAssignments(VAR_NAME v, EXPRESSION e, EXPRESSION_TABLE* data);
    //string RemoveWhiteSpace(EXPRESSION );
    bool HasMatchingPattern(TNode root, TNode qroot);
    TNode* ParseExpression(EXPRESSION s);
    bool IsNumber(EXPRESSION s);
public:
    void AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root);

    STMT_IDX_SET GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_IDX_SET GetAssignWithSubpattern(VAR_NAME v, EXPRESSION e);
    STMT_VAR_PAIR_LIST GetAssignStmtVarPairWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_VAR_PAIR_LIST GetAssignStmtVarPairWithSubPattern(VAR_NAME v, EXPRESSION e);

};

#endif // PATTERNMANAGER_H