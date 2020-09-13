#ifndef _CUSTOMTYPES_H_
#define _CUSTOMTYPES_H_

#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#define TYPE_CONST "constant"
#define TYPE_PROC "procedure"
#define TYPE_VAR "variable"
#define TYPE_STMT "statement"

#define TYPE_STMT_ASSIGN "assign"
#define TYPE_STMT_CALL "call"
#define TYPE_STMT_IF "if"
#define TYPE_STMT_WHILE "while"
#define TYPE_STMT_PRINT "print"
#define TYPE_STMT_READ "read"

#define TYPE_COND_FOLLOWS "Follows"
#define TYPE_COND_FOLLOWS_T "FollowsT"
#define TYPE_COND_PARENT "Parent"
#define TYPE_COND_PARENT_T "ParentT"
#define TYPE_COND_USES_S "UsesS"
#define TYPE_COND_USES_P "UsesP"
#define TYPE_COND_MODIFIES_S "ModifiesS"
#define TYPE_COND_MODIFIES_P "ModifiesP"
#define TYPE_COND_PATTERN "pattern"

typedef std::string VAR_NAME;
typedef std::string PROC_NAME;
typedef int STMT_IDX;
typedef double CONST_VALUE;
typedef std::string EXPRESSION;
typedef std::vector<VAR_NAME*> VAR_NAME_LIST;
typedef std::vector<PROC_NAME*> PROC_NAME_LIST;
typedef std::vector<STMT_IDX> STMT_IDX_LIST;
typedef std::unordered_set<VAR_NAME*> VAR_NAME_SET;
typedef std::unordered_set<PROC_NAME*> PROC_NAME_SET;
typedef std::unordered_set<STMT_IDX> STMT_IDX_SET;
typedef std::unordered_set<CONST_VALUE> CONST_VALUE_SET;
typedef std::tuple<STMT_IDX, STMT_IDX> STMT_STMT_PAIR;
typedef std::tuple<STMT_IDX, VAR_NAME*> STMT_VAR_PAIR;
typedef std::tuple<PROC_NAME*, VAR_NAME*> PROC_VAR_PAIR;
typedef std::unordered_set<STMT_STMT_PAIR*> STMT_STMT_PAIR_SET;
typedef std::unordered_set<STMT_VAR_PAIR*> STMT_VAR_PAIR_SET;
typedef std::unordered_set<PROC_VAR_PAIR*> PROC_VAR_PAIR_SET;
typedef std::unordered_map<STMT_IDX, STMT_IDX_SET*> STMT_STMT_RELATION_TABLE;
typedef std::unordered_map<STMT_IDX, VAR_NAME_SET*> STMT_VAR_RELATION_TABLE;
typedef std::unordered_map<VAR_NAME*, STMT_IDX_SET*> VAR_STMT_RELATION_TABLE;
typedef std::unordered_map<PROC_NAME*, VAR_NAME_SET*> PROC_VAR_RELATION_TABLE;
typedef std::unordered_map<VAR_NAME*, PROC_NAME_SET*> VAR_PROC_RELATION_TABLE;
typedef STMT_STMT_RELATION_TABLE FOLLOWS_TABLE;
typedef STMT_STMT_RELATION_TABLE FOLLOWS_STAR_TABLE;
typedef STMT_STMT_RELATION_TABLE INVERSE_FOLLOWS_TABLE;
typedef STMT_STMT_RELATION_TABLE INVERSE_FOLLOWS_STAR_TABLE;
typedef STMT_STMT_RELATION_TABLE PARENT_TABLE;
typedef STMT_STMT_RELATION_TABLE PARENT_STAR_TABLE;
typedef STMT_STMT_RELATION_TABLE INVERSE_PARENT_TABLE;
typedef STMT_STMT_RELATION_TABLE INVERSE_PARENT_STAR_TABLE;
typedef PROC_VAR_RELATION_TABLE PROC_USES_TABLE;
typedef VAR_PROC_RELATION_TABLE INVERSE_PROC_USES_TABLE;
typedef STMT_VAR_RELATION_TABLE STMT_USES_TABLE;
typedef VAR_STMT_RELATION_TABLE INVERSE_STMT_USES_TABLE;
typedef PROC_VAR_RELATION_TABLE PROC_MODIFIES_TABLE;
typedef VAR_PROC_RELATION_TABLE INVERSE_PROC_MODIFIES_TABLE;
typedef STMT_VAR_RELATION_TABLE STMT_MODIFIES_TABLE;
typedef VAR_STMT_RELATION_TABLE INVERSE_STMT_MODIFIES_TABLE;

enum ENTITY_TYPE {
	statement,
	variable,
	proc,
	constant,
};

enum STATEMENT_TYPE {
	assignStatement,
	ifStatement,
	whileStatement,
	readStatement,
	printStatement,
	callStatement,
};
/*
enum RELREF {
	Follows,
	FollowsT,
	Parent,
	ParentT,
	UsesS, 
	UsesP,
	ModifiesS,
	ModifiesP,
	pattern,
};

struct ENTITY {
	ENTITY_TYPE type;
};

struct STATEMENT: ENTITY {
	ENTITY_TYPE type = ENTITY_TYPE::statement;
	STATEMENT_TYPE statement_type; 
	STMT_IDX idx;
};

struct VARIABLE: ENTITY {
	ENTITY_TYPE type = ENTITY_TYPE::variable;
	VAR_NAME name;
};

struct PROCEDURE : ENTITY {
	ENTITY_TYPE type = ENTITY_TYPE::proc;
	PROC_NAME name;
};

struct CONSTANT : ENTITY {
	ENTITY_TYPE type = ENTITY_TYPE::constant;
	CONST_VALUE value;
};
*/
//typedef std::vector<STATEMENT> STMT_LIST;
//typedef std::vector<VARIABLE> VARIABLE_LIST;
//typedef std::vector<PROCEDURE> PROCEDURE_LIST;
//typedef std::vector<ENTITY> ENTITY_LIST;

#endif