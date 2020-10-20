#pragma once

#include "PQLCustomTypes.h"

class QueryRules
{
	/*
		Class to Define all Query Rules. Provides all the helper functions for the QuerySyntaxValidator.
		Provides all the functions to check each token.
	*/
public:
	// Lexical Tokens
	BOOLEAN IsIdent(STRING token);
	BOOLEAN IsName(STRING token);
	BOOLEAN IsInteger(STRING token);
	BOOLEAN IsSynonym(STRING token);
	BOOLEAN IsStmtRef(STRING token, STRING synonym_type);
	BOOLEAN IsEntRef(STRING token, STRING synonym_type);
	BOOLEAN IsLineRef(STRING token, STRING synonym_type);
	BOOLEAN IsElem(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsAttrName(STRING token);
	BOOLEAN IsDesignEntity(STRING token);

	// Grammar Rules
	BOOLEAN IsSelect(STRING token);
	BOOLEAN IsDeclaration(STRING token, STRING_STRING_MAP entity_map);
	BOOLEAN IsResultClause(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsTuple(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsWithClause(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsSuchThatClause(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsPatternClause(STRING token, STRING_STRING_MAP declared_var_names);

	BOOLEAN IsAttrCond(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsAttrCompare(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsRef(STRING token, STRING synonym_type);
	BOOLEAN IsAttrRef(STRING token, STRING synonym_type);

	BOOLEAN IsRelCond(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsRelRef(STRING token, STRING_STRING_MAP declared_var_names);
	STRING GetRelRefType(STRING token, STRING_STRING_MAP declared_var_names);

	BOOLEAN IsModifiesP(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsModifiesS(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsUsesP(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsUsesS(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsCalls(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsCallsT(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsParent(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsParentT(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsFollows(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsFollowsT(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsNext(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsNextT(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsAffects(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsAffectsT(STRING token, STRING_STRING_MAP declared_var_names);

	BOOLEAN IsPatternCond(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsPattern(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsAssign(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsExpressionSpec(STRING token);
	BOOLEAN IsOperator(CHAR c);
	BOOLEAN IsExpr(STRING token);
	BOOLEAN IsTerm(STRING token);
	BOOLEAN IsFactor(STRING token);
	BOOLEAN IsVarName(STRING token);
	BOOLEAN IsConstValue(STRING token);
	BOOLEAN IsSynIf(STRING token, STRING_STRING_MAP declared_var_names);
	BOOLEAN IsSynWhile(STRING token, STRING_STRING_MAP declared_var_names);
};
