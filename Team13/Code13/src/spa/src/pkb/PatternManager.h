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
    EXPRESSION_TABLE data_;
public:
    EXPRESSION_TABLE GetData();
    void Add(STMT_IDX s, VAR_NAME v, TNode root);
};

class PatternManager {
private:
    StatementTable statement_table_;
    RelationManager relation_manager_;
    AssignPatternTable assign_pattern_table_;

    STMT_IDX_LIST GetStmtIdxSetIntersection(STMT_IDX_SET s1, STMT_IDX_SET s2);
    STMT_IDX_LIST GetStmtIdxSetIntersection(STMT_IDX_LIST l1, STMT_IDX_SET s2);

    STMT_IDX_SET GetAssignWithPattern(VAR_NAME v, EXPRESSION e, AssignPatternTable table);
    //string RemoveWhiteSpace(EXPRESSION );
    bool HasMatchingPattern(TNode root, TNode qroot);
    TNode* ParseExpression(EXPRESSION s);
    bool IsNumber(EXPRESSION s);
public:
    PatternManager(StatementTable statement_table, RelationManager relation_manager) {
        this->statement_table_ = statement_table;
        this->relation_manager_ = relation_manager;
    }
    void AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root);

    STMT_IDX_SET GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_IDX_SET GetAssignWithSubpattern(VAR_NAME v, EXPRESSION e);
    STMT_VAR_PAIR_LIST GetAssignStmtVarPairWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_VAR_PAIR_LIST GetAssignStmtVarPairWithSubPattern(VAR_NAME v, EXPRESSION e);

};

#endif // PATTERNMANAGER_H