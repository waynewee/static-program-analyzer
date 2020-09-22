#ifndef _PQL_CUSTOMTYPES_H_
#define _PQL_CUSTOMTYPES_H_

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#define DEBUG true

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
#define TYPE_COND_PATTERN_P "pattern_p"
#define TYPE_COND_PATTERN_F "pattern_f"

typedef string STRING;
typedef int INTEGER;
typedef double DOUBLE;
typedef bool BOOLEAN;
typedef void VOID;

typedef vector<STRING> STRING_LIST;
typedef vector<STRING_LIST> STRINGLIST_LIST;

typedef unordered_set<STRING> STRING_SET;
typedef unordered_set<STRING_LIST*> STRINGLIST_SET;

typedef unordered_set<INTEGER> INTEGER_SET;
typedef unordered_set<DOUBLE> DOUBLE_SET;

typedef unordered_map<STRING, STRING> STRING_STRING_MAP;
typedef unordered_map<STRING, STRING_LIST> STRING_STRINGLIST_MAP;
typedef unordered_map<STRING, STRING_SET> STRING_STRINGSET_MAP;
typedef unordered_map<STRING, STRINGLIST_LIST> STRING_STRINGLISTLIST_MAP;

typedef unordered_map<STRING_LIST*, STRING_LIST> STRINGLIST_STRINGLIST_MAP;
typedef unordered_map<STRING_LIST*, STRINGLIST_SET> STRINGLIST_STRINGLISTSET_MAP;

#endif
