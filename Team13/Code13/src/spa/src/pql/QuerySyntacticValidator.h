#include "PQLCustomTypes.h"

#pragma once

/*
The QuerySyntacticValidator only validates if a given query is valid syntactically and does not check for Semantics.

*/
class QuerySyntacticValidator
{
public:
	QuerySyntacticValidator(); 

	BOOLEAN ValidateDeclaration(STRING token);
	BOOLEAN ValidateSuchThatClause(STRING token);

	BOOLEAN ValidatePatternClause(STRING token, STRING_STRING_MAP declared_var_names);

	BOOLEAN ValidateWithClause(STRING token);

	BOOLEAN IsRelRef(STRING token);
	BOOLEAN IsValidRelRefArgument(STRING token);
	BOOLEAN IsIdent(STRING token);
	BOOLEAN IsName(STRING token);
	BOOLEAN IsInteger(STRING token);
	BOOLEAN IsSynonym(STRING token);

	BOOLEAN IsAttrRef(STRING token);
	BOOLEAN IsRef(STRING token);
};

