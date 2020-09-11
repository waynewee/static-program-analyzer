#ifndef _CUSTOMTYPES_H_
#define _CUSTOMTYPES_H_

#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_set>

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

//typedef std::vector<STATEMENT> STMT_LIST;
//typedef std::vector<VARIABLE> VARIABLE_LIST;
//typedef std::vector<PROCEDURE> PROCEDURE_LIST;
//typedef std::vector<ENTITY> ENTITY_LIST;

#endif