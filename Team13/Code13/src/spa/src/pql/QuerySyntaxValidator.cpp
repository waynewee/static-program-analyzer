#include "PQLCustomTypes.h"
#include "QuerySyntaxValidator.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <iostream>
using namespace std;

QuerySyntaxValidator::QuerySyntaxValidator() {
	QueryRules query_rules;
	this->query_rules_ = query_rules;
}

BOOLEAN QuerySyntaxValidator::ValidateDeclaration(STRING token) {
	return this->query_rules_.IsDeclaration(token);
}

BOOLEAN QuerySyntaxValidator::ValidateSelect(STRING token) {
	return this->query_rules_.IsSelect(token);
}

BOOLEAN QuerySyntaxValidator::ValidateResultClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsResultClause(token, user_declared_vars);
}

BOOLEAN QuerySyntaxValidator::ValidateSuchthatClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsSuchThatClause(token, user_declared_vars);
}

STRING QuerySyntaxValidator::GetValidRelRefType(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.GetRelRefType(token, user_declared_vars);
}

BOOLEAN QuerySyntaxValidator::ValidatePatternClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsPatternClause(token, user_declared_vars);
}

BOOLEAN QuerySyntaxValidator::ValidateWithClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsWithClause(token, user_declared_vars);
}