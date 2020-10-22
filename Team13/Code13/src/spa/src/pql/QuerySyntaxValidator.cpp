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

bool QuerySyntaxValidator::ValidateDeclaration(string token, STRING_STRING_MAP entity_map) {
	return this->query_rules_.IsDeclaration(token, entity_map);
}

bool QuerySyntaxValidator::ValidateSelect(string token) {
	return this->query_rules_.IsSelect(token);
}

bool QuerySyntaxValidator::ValidateResultClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsResultClause(token, user_declared_vars);
}

bool QuerySyntaxValidator::ValidateSuchthatClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsSuchThatClause(token, user_declared_vars);
}

string QuerySyntaxValidator::GetValidRelRefType(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.GetRelRefType(token, user_declared_vars);
}

bool QuerySyntaxValidator::ValidatePatternClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsPatternClause(token, user_declared_vars);
}

bool QuerySyntaxValidator::ValidateWithClause(string token, STRING_STRING_MAP user_declared_vars) {
	return this->query_rules_.IsWithClause(token, user_declared_vars);
}