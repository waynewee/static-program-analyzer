#ifndef _TOKENTYPE_H_
#define _TOKENTYPE_H_

#include <string>

using namespace std;

class TokenType{
public:
	
	enum class STMT_TYPE { 
		_procedure, 
		_call,
		_read, 
		_while, 
		_print, 
		_if, 
		_then, 
		_else 
	};

	enum class TOKEN_TYPE { 
		stmt, 
		punc, 
		rel_expr, 
		expr, 
		constant, 
		var 
	};

	static STMT_TYPE GetStmtType(string tokenStr);
};

#endif