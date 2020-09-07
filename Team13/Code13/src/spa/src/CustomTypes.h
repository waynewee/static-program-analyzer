#include <string>
#include <cstdlib>
#include <vector>

typedef std::string VAR_NAME;
typedef std::string PROC_NAME;
typedef int STMT_IDX;
typedef double CONST_VALUE;
typedef std::string EXPRESSION;
typedef std::vector<VAR_NAME> LIST_OF_VAR_NAME;
typedef std::vector<STMT_IDX> LIST_OF_STMT_IDX;

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

typedef std::vector<STATEMENT> STATEMENT_LIST;
typedef std::vector<VARIABLE> VARIABLE_LIST;
typedef std::vector<PROCEDURE> PROCEDURE_LIST;
typedef std::vector<ENTITY> ENTITY_LIST;