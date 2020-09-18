#ifndef _PQL_CUSTOMTYPES_H_
#define _PQL_CUSTOMTYPES_H_

#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

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

typedef string STRING;
typedef int INTEGER;
typedef double DOUBLE;
typedef bool BOOLEAN;

typedef STRING* STRING_PTR;

typedef vector<STRING_PTR> STRING_LIST;
typedef STRING_LIST* STRING_LIST_PTR;

typedef vector<STRING_LIST_PTR> STRINGLIST_LIST;
typedef vector<STRING_LIST_PTR>* STRINGLIST_LIST_PTR;

typedef unordered_set<STRING_PTR> STRING_SET;
typedef STRING_SET* STRING_SET_PTR;

typedef unordered_set<STRING_LIST_PTR> STRINGLIST_SET;
typedef STRINGLIST_SET* STRINGLIST_SET_PTR;

typedef unordered_set<INTEGER> INTEGER_SET;
typedef INTEGER_SET* INTEGER_SET_PTR;

typedef unordered_set<DOUBLE> DOUBLE_SET;
typedef DOUBLE_SET* DOUBLE_SET_PTR;

typedef unordered_map<STRING_PTR, STRING_SET_PTR> STRING_STRINGSET_MAP;
typedef STRING_STRINGSET_MAP* STRING_STRINGSET_MAP_PTR;

typedef unordered_map<STRING_LIST_PTR, STRINGLIST_SET_PTR> STRINGLIST_STRINGLISTSET_MAP;
typedef STRINGLIST_STRINGLISTSET_MAP* STRINGLIST_STRINGLISTSET_MAP_PTR;

typedef unordered_map<STRING_LIST_PTR, STRING_LIST_PTR> STRINGLIST_STRINGLIST_MAP;
typedef STRINGLIST_STRINGLIST_MAP* STRINGLIST_STRINGLIST_MAP_PTR;

typedef unordered_map<STRING_PTR, STRING_PTR> STRING_STRING_MAP;
typedef STRING_STRING_MAP* STRING_STRING_MAP_PTR;

typedef unordered_map<STRING_PTR, STRINGLIST_LIST_PTR> STRING_STRINGLISTLIST_MAP;
typedef STRING_STRINGLISTLIST_MAP* STRING_STRINGLISTLIST_MAP_PTR;

#endif