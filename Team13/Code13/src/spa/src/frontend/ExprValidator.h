#ifndef _EXPRVALIDATOR_H
#define _EXPRVALIDATOR_H

#include <FrontendTypes.h>

using namespace std;

class ExprValidator {
public:
	static void Validate(TOKEN_LIST expr_list);

private:
	static void CheckValid(Token prevToken, Token currToken);

};

#endif