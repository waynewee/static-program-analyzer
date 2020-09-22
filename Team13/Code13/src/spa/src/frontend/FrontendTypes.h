#ifndef _FRONTENDTYPES_H_
#define _FRONTENDTYPES_H_

#include <vector>

#include "Token.h"

#define TYPE_STMT_IF_THEN "then"
#define TYPE_STMT_IF_ELSE "else"

#define TYPE_PUNC_OPEN_PARAN "("
#define TYPE_PUNC_CLOSED_PARAN ")"
#define TYPE_PUNC_SEMICOLON ";"
#define TYPE_PUNC_OPEN_BRACKET "{"
#define TYPE_PUNC_CLOSED_BRACKET "}"

#define TYPE_EQUALS "="


typedef vector<string> STMT_NAME_LIST;
typedef vector<Token> TOKEN_LIST;
typedef vector<string> STR_LIST;

#endif