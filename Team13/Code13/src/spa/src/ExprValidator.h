#ifndef _EXPRVALIDATOR_H
#define _EXPRVALIDATOR_H

#include <FrontendTypes.h>

using namespace std;

class ExprValidator {
public:
	static bool Validate(TOKEN_LIST expr_list);

private:
	static bool CheckValid(Token prevToken, Token currToken);

};

#endif