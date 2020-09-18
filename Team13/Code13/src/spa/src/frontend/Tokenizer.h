#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <map>
#include <vector>

#include <CustomTypes.h>
#include <FrontendTypes.h>
#include <Token.h>
#include <TokenType.h>

using namespace std;

#define WHITESPACE_NEWLINE "\n"
#define WHITESPACE_SPACE " "
#define WHITESPACE_RETURN "\r"
#define WHITESPACE_TAB "\t"

#define TYPE_REL_EXPR_AND_PART "&"
#define TYPE_REL_EXPR_EQ_PART "="
#define TYPE_REL_EXPR_GT_PART ">"
#define TYPE_REL_EXPR_LT_PART "<"
#define TYPE_REL_EXPR_NEQ_PART "!"
#define TYPE_REL_EXPR_OR_PART "|"

class Tokenizer {
public:
	
	int len;
	int pos;
	string text;
	string tokenStr;

	STR_LIST expressions = {
		TYPE_EXPR_MOD,
		TYPE_EXPR_PLUS,
		TYPE_EXPR_MINUS,
		TYPE_EXPR_TIMES,
		TYPE_EXPR_DIVIDE
	};

	STR_LIST punctuations = {
		TYPE_PUNC_CLOSED_BRACKET,
		TYPE_PUNC_CLOSED_PARAN,
		TYPE_PUNC_OPEN_BRACKET,
		TYPE_PUNC_OPEN_PARAN,
		TYPE_PUNC_SEMICOLON
	};
	
	STR_LIST rel_expressions = {
		TYPE_REL_EXPR_GT,
		TYPE_REL_EXPR_GTE,
		TYPE_REL_EXPR_LT,
		TYPE_REL_EXPR_LTE,
		TYPE_REL_EXPR_EQ,
		TYPE_REL_EXPR_AND,
		TYPE_REL_EXPR_OR,
		TYPE_REL_EXPR_NEQ,
		TYPE_REL_EXPR_NOT
	};

	STR_LIST rel_expressions_part = {
		TYPE_REL_EXPR_GT_PART ,
		TYPE_REL_EXPR_LT_PART ,
		TYPE_REL_EXPR_EQ_PART ,
		TYPE_REL_EXPR_AND_PART,
		TYPE_REL_EXPR_OR_PART ,
		TYPE_REL_EXPR_NEQ_PART
	};

	STMT_NAME_LIST stmtNames = { 
		TYPE_PROC, 
		TYPE_STMT_PRINT,
		TYPE_STMT_READ,
		TYPE_STMT_CALL, 
		TYPE_STMT_WHILE, 
		TYPE_STMT_IF, 
		TYPE_STMT_IF_THEN,
		TYPE_STMT_IF_ELSE
	};

	STR_LIST whiteSpaces = {
		WHITESPACE_NEWLINE,
		WHITESPACE_RETURN,
		WHITESPACE_SPACE,
		WHITESPACE_TAB
	};

	TOKEN_LIST tokenList;

	Tokenizer(string input);
	
	int Tokenize();

	Token* AddToken(TokenType::TOKEN_TYPE tokenType);
	void AppendStrToTokenStr(string str);
	void AppendCharToTokenStr(char c);
	vector<Token> GetTokenList();
	void PrintToken(string type);
	void PrintTokenList();
	void ResetTokenStr();
	void TestAndSetUnary(Token* currPtr, Token prev);

	bool CheckMatch(string s, STR_LIST v);
	bool IsExpr(char c);
	bool IsPunc(char c);
	bool IsRelExpr(string str);
	bool IsRelExprPart(char c);
	bool IsWhiteSpace(char c);

};

#endif