#include "PQLCustomTypes.h"
#include "QueryValidator.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <iostream>
using namespace std;

QueryValidator::QueryValidator() {
	QueryRules query_rules;
	this->query_rules_ = query_rules;
}

BOOLEAN QueryValidator::ValidateDeclaration(STRING token, STRING_STRING_MAP entity_map) {
	return this->query_rules_.IsDeclaration(token, entity_map);
}

BOOLEAN QueryValidator::ValidateSelect(STRING token) {
	return this->query_rules_.IsSelect(token);
}

BOOLEAN QueryValidator::ValidateResultClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsResultClause(token, user_declared_vars);
}

BOOLEAN QueryValidator::ValidateSuchthatClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsSuchThatClause(token, user_declared_vars);
}

STRING QueryValidator::GetValidRelRefType(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.GetRelRefType(token, user_declared_vars);
}

BOOLEAN QueryValidator::ValidatePatternClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsPatternClause(token, user_declared_vars);
}

BOOLEAN QueryValidator::ValidateWithClause(STRING token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsWithClause(token, user_declared_vars);
}