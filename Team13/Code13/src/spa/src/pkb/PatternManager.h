#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <cstdlib>
#include <stack>
#include <algorithm>
#include <CustomTypes.h>
#include <DataTables.h>
#include <RelationManager.h>

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
    STMT_IDX_LIST GetAllAssignModifies(VAR_NAME v);
    STMT_IDX_SET GetAssignWithPattern(VAR_NAME v, EXPRESSION e, AssignPatternTable table);

    STMT_IDX_SET GetAssignWithMatchingPattern(TNode* qroot);
    bool MatchPattern(TNode* root, TNode* qroot);
    string RemoveWhiteSpace(EXPRESSION e);
    TNode* ParseExpression(EXPRESSION e);
    bool IsNumber(EXPRESSION s);
public:
    PatternManager(StatementTable statement_table, RelationManager relation_manager) {
        this->statement_table_ = statement_table;
        this->relation_manager_ = relation_manager;
    }
    void AddAssignPattern(STMT_IDX s, VAR_NAME v, TNode root);

    STMT_IDX_SET GetAssignWithFullPattern(VAR_NAME v, EXPRESSION e);
    STMT_IDX_SET GetAssignWithSubpattern(VAR_NAME v, EXPRESSION e);
};

#endif // PATTERNMANAGER_H