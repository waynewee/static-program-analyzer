#ifndef RELATION_MANAGER_H
#define RELATION_MANAGER_H

#include <unordered_map>
#include <CustomTypes.h>

class RelationManager {
private:
    static FOLLOWS_TABLE* follows_table_;
    static FOLLOWS_STAR_TABLE *follows_star_table_;
    static INVERSE_FOLLOWS_TABLE *inverse_follows_table_;
    static INVERSE_FOLLOWS_STAR_TABLE *inverse_follows_star_table_;
    static PARENT_TABLE *parent_table_;
    static PARENT_STAR_TABLE *parent_star_table_;
    static INVERSE_PARENT_TABLE *inverse_parent_table_;
    static INVERSE_PARENT_STAR_TABLE *inverse_parent_star_table_;
    static STMT_USES_TABLE *stmt_uses_table_;
    static PROC_USES_TABLE *proc_uses_table_;
    static INVERSE_STMT_USES_TABLE *inverse_stmt_uses_table_;
    static INVERSE_PROC_USES_TABLE *inverse_proc_uses_table_;
    static STMT_MODIFIES_TABLE *stmt_modifies_table_;
    static PROC_MODIFIES_TABLE *proc_modifies_table_;
    static INVERSE_STMT_MODIFIES_TABLE *inverse_stmt_modifies_table_;
    static INVERSE_PROC_MODIFIES_TABLE *inverse_proc_modifies_table_;
    static STMT_STMT_PAIR_SET *all_follows_;
    static STMT_STMT_PAIR_SET *all_follows_star_;
    static STMT_STMT_PAIR_SET *all_parent_;
    static STMT_STMT_PAIR_SET *all_parent_star_;
    static STMT_VAR_PAIR_SET *all_stmt_uses_;
    static PROC_VAR_PAIR_SET *all_proc_uses_;
    static STMT_VAR_PAIR_SET *all_stmt_modifies_;
    static PROC_VAR_PAIR_SET *all_proc_modifies_;

    bool InsertStmtStmtRelation(STMT_STMT_RELATION_TABLE *set, STMT_IDX s1, STMT_IDX s2);
    bool InsertStmtVarRelation(STMT_VAR_RELATION_TABLE *set, STMT_IDX s, VAR_NAME *v);
    bool InsertProcVarRelation(PROC_VAR_RELATION_TABLE *set, PROC_NAME *p, VAR_NAME *v);
    bool InsertVarStmtRelation(VAR_STMT_RELATION_TABLE *set, VAR_NAME *v, STMT_IDX s);
    bool InsertVarProcRelation(VAR_PROC_RELATION_TABLE *set, VAR_NAME *v, PROC_NAME *p);

    bool InsertStmtStmtTuple(STMT_STMT_PAIR_SET *set, STMT_IDX s1, STMT_IDX s2);
    bool InsertStmtVarTuple(STMT_VAR_PAIR_SET *set, STMT_IDX s, VAR_NAME *v);
    bool InsertProcVarTuple(PROC_VAR_PAIR_SET *set, PROC_NAME *p, VAR_NAME *v);

    bool CheckStmtStmtRelation(STMT_STMT_RELATION_TABLE *set, STMT_IDX s1, STMT_IDX s2);
    bool CheckStmtVarRelation(STMT_VAR_RELATION_TABLE *set, STMT_IDX s, VAR_NAME *v);
    bool CheckProcVarRelation(PROC_VAR_RELATION_TABLE *set, PROC_NAME *p, VAR_NAME *v);
    bool CheckVarStmtRelation(VAR_STMT_RELATION_TABLE *set, VAR_NAME *v, STMT_IDX s);
    bool CheckVarProcRelation(VAR_PROC_RELATION_TABLE *set, VAR_NAME *v, PROC_NAME *p);

    STMT_IDX_SET* GetStmtStmtRelationVal(STMT_STMT_RELATION_TABLE *set, STMT_IDX s);
    VAR_NAME_SET* GetStmtVarRelationVal(STMT_VAR_RELATION_TABLE *set, STMT_IDX s);
    VAR_NAME_SET* GetProcVarRelationVal(PROC_VAR_RELATION_TABLE *set, PROC_NAME *p);
    STMT_IDX_SET* GetVarStmtRelationVal(VAR_STMT_RELATION_TABLE *set, VAR_NAME *v);
    PROC_NAME_SET* GetVarProcRelationVal(VAR_PROC_RELATION_TABLE *set, VAR_NAME *v);
public:
    bool AddFollows(STMT_IDX s1, STMT_IDX s2);
    bool AddFollowsStar(STMT_IDX s1, STMT_IDX s2);
    bool AddParent(STMT_IDX s1, STMT_IDX s2);
    bool AddParentStar(STMT_IDX s1, STMT_IDX s2);
    bool AddStmtUses(STMT_IDX s, VAR_NAME* v);
    bool AddProcUses(PROC_NAME* p, VAR_NAME* v);
    bool AddStmtModifies(STMT_IDX s, VAR_NAME* v);
    bool AddProcModifies(PROC_NAME* p, VAR_NAME* v);

    bool IsFollows(STMT_IDX s1, STMT_IDX s2);
    bool IsFollowsStar(STMT_IDX s1, STMT_IDX s2);
    bool IsParent(STMT_IDX s1, STMT_IDX s2);
    bool IsParentStar(STMT_IDX s1, STMT_IDX s2);
    bool IsStmtUses(STMT_IDX s, VAR_NAME *v);
    bool IsProcUses(PROC_NAME *p, VAR_NAME *v);
    bool IsStmtModifies(STMT_IDX s, VAR_NAME *v);
    bool IsProcModifies(PROC_NAME *p, VAR_NAME *v);

    STMT_IDX_SET* GetFollows(STMT_IDX s);
    STMT_IDX_SET* GetInverseFollows(STMT_IDX s);
    STMT_STMT_PAIR_SET* GetAllFollows();
    STMT_IDX_SET* GetFollowsStars(STMT_IDX s);
    STMT_IDX_SET* GetInverseFollowsStars(STMT_IDX s);
    STMT_STMT_PAIR_SET* GetAllFollowsStars();
    STMT_IDX_SET* GetParents(STMT_IDX s);
    STMT_IDX_SET* GetInverseParents(STMT_IDX s);
    STMT_STMT_PAIR_SET* GetAllParents();
    STMT_IDX_SET* GetParentStars(STMT_IDX s);
    STMT_IDX_SET* GetInverseParentStars(STMT_IDX s);
    STMT_STMT_PAIR_SET* GetAllParentStars();
    VAR_NAME_SET* GetStmtUses(STMT_IDX s);
    STMT_IDX_SET* GetInverseStmtUses(VAR_NAME *v);
    STMT_VAR_PAIR_SET* GetAllStmtUses();
    VAR_NAME_SET* GetProcUses(PROC_NAME *p);
    PROC_NAME_SET* GetInverseProcUses(VAR_NAME *v);
    PROC_VAR_PAIR_SET* GetAllProcUses();
    VAR_NAME_SET* GetStmtModifies(STMT_IDX s);
    STMT_IDX_SET* GetInverseStmtModifies(VAR_NAME *v);
    STMT_VAR_PAIR_SET* GetAllStmtModifies();
    VAR_NAME_SET* GetProcModifies(PROC_NAME *p);
    PROC_NAME_SET* GetInverseProcModifies(VAR_NAME *v);
    PROC_VAR_PAIR_SET* GetAllProcModifies();

    RelationManager() {
    }
};

#endif // RELATION_MANAGER_H