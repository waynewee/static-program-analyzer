#pragma once

#include <string>

class TokenType{
public:
	
	enum class TOKEN_TYPE { stmt, punc, rel_expr, expr, constant, var };

	enum class STMT_TYPE { _procedure, _call, _read, _while, _print, _if, _then, _else };

	static STMT_TYPE getStmtType(std::string tokenStr);
};